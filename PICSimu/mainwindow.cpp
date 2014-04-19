#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"
#include "codeline.h"
#include <iostream>
#include <QDialog>
#include <sstream>
#include <string>
#include <QFileDialog>
#include "goklasse.h"

#define n_register 48

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    steuerwerk = NULL;

    // Schriftart für ListWidget festlegen
    QFont font = QFont ("Courier");
    font.setStyleHint (QFont::Monospace);
    font.setPointSize (8);
    font.setFixedPitch (true);
    ui->lw_lstFile->setFont(font);

    // SLOT-SIGNAL-Verbindungen
    connect(ui->pb_load, SIGNAL(clicked()), SLOT(slotLoadLstFile()));
    connect(ui->selectFile_Button, SIGNAL(clicked()),SLOT(slotLoad_FileDialog()));
    connect(ui->refresh_speicher, SIGNAL(clicked()), SLOT(slotRefreshSpeicher()));
    connect(ui->pb_executeStep, SIGNAL(clicked()), SLOT(slotExecuteStep()));
	connect(ui->resetButton,SIGNAL(clicked()),SLOT(slotResetClicked()));
    connect(ui->goButton,SIGNAL(clicked()),SLOT(slotGoClicked()));

    // Mario
    // Tabelle für Speicherausgabe definieren
    ui->tw_speicher->setRowCount(2*n_register);
    ui->tw_speicher->setColumnCount(3);
    ui->tw_speicher->verticalHeader()->setVisible(false);
    ui->tw_speicher->setColumnWidth(0,35);
    ui->tw_speicher->setColumnWidth(1,35);
    ui->tw_speicher->setHorizontalHeaderItem(0,new QTableWidgetItem("#"));
    ui->tw_speicher->setHorizontalHeaderItem(1,new QTableWidgetItem("Hex"));
    ui->tw_speicher->setHorizontalHeaderItem(2,new QTableWidgetItem("Bin"));
	ui->tw_speicher->setFont(font);

    initializeSpeicherWidget();
    // Mario ende

    ui->tw_specialFunctionRegister->setRowCount(8);
    ui->tw_specialFunctionRegister->setColumnCount(3);
    ui->tw_specialFunctionRegister->verticalHeader()->setVisible(false);

    ui->tw_specialFunctionRegister->setColumnWidth(0,60);
    ui->tw_specialFunctionRegister->setColumnWidth(1,35);

    ui->tw_specialFunctionRegister->setHorizontalHeaderItem(0,new QTableWidgetItem("Name"));
    ui->tw_specialFunctionRegister->setHorizontalHeaderItem(1,new QTableWidgetItem("Hex"));
    ui->tw_specialFunctionRegister->setHorizontalHeaderItem(2,new QTableWidgetItem("Bin"));
    ui->tw_speicher->setFont(font);

    initializeSFRWidget();

    //Verzögerungs spinbox initialisieren
    ui->verzoegerung->setMaximum(250);
    ui->verzoegerung->setValue(100);
    ui->verzoegerung->setMinimum(50);
    ui->verzoegerung->setHidden(true);
    ui->label_verzoegerung->setHidden(true);

    //Initialisieren Frequenz
    ui->frequency->setValue(10);
    ui->frequency->setMinimum(1);
    ui->cycle->setText(QString::fromStdString("0.4"));
    ui->runtime->setText(QString::fromStdString("0"));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSteuerwerk(Steuerwerk* steuerwerk)
{
    this->steuerwerk = steuerwerk;
}

void MainWindow::slotLoadLstFile() // Benedikt: geändert
{
    // cout << "slotLoadLstFile() gestartet" << endl;
	// evtl. vorhandene Inhalte löschen
	steuerwerk->clearProgrammspeicher();
	steuerwerk->clearSteuerwerk();
	ui->lw_lstFile->clear();    // LineWidget leeren
	slotRefreshSpeicher();

    // refreshSFRWidget();

    if(steuerwerk->loadFile(ui->le_filename->text().toStdString()))  // auslesen erfolgreich?
    {
        ui->lw_lstFile->clear();    // LineWidget leeren

        // cout << "Ausgabe gestartet" << endl;

		for(list<QString>::iterator it=steuerwerk->lstFile.begin(); it!=steuerwerk->lstFile.end(); it++)
		{
			ui->lw_lstFile->addItem(*it);   // Code zeilenweise in ListWidget einfügen
		}

		//ersten Befehl einfärben

        setLineColorGreen(steuerwerk->getCurrentLineNumber()-1);

    }
    else
        ui->lw_lstFile->addItem("File does not exist!");
    // cout << "slotLoadLstFile() beendet" << endl;
    slotResetClicked();
}

void MainWindow::slotLoad_FileDialog()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
													 "",
													 tr("Files (*.*)"));
	ui->le_filename->clear();
	ui->le_filename->setText(fileName);
}

// Mario
void MainWindow::initializeSpeicherWidget()
{
    for(int i=0; i < n_register; i++)
    {
        ui->tw_speicher->setItem(i, 0, new QTableWidgetItem(QString::number(i,16)));
        ui->tw_speicher->setItem(i+n_register, 0, new QTableWidgetItem(QString::number(i+128,16)));

        ui->tw_speicher->setItem(i, 1, new QTableWidgetItem("0"));
        ui->tw_speicher->setItem(i+n_register, 1, new QTableWidgetItem("0"));

        ui->tw_speicher->setItem(i, 2, new QTableWidgetItem("0"));
        ui->tw_speicher->setItem(i+n_register, 2, new QTableWidgetItem("0"));

        ui->tw_speicher->item(i,2)->setTextAlignment(Qt::AlignRight);
        ui->tw_speicher->item(i+n_register,2)->setTextAlignment(Qt::AlignRight);
    }
}

void MainWindow::initializeSFRWidget()
{
    ui->tw_specialFunctionRegister->setItem(0, 0, new QTableWidgetItem("W-Reg"));
    ui->tw_specialFunctionRegister->setItem(1, 0, new QTableWidgetItem("FSR"));
    ui->tw_specialFunctionRegister->setItem(2, 0, new QTableWidgetItem("PCL"));
    ui->tw_specialFunctionRegister->setItem(3, 0, new QTableWidgetItem("PCLATH"));
    ui->tw_specialFunctionRegister->setItem(4, 0, new QTableWidgetItem("PC"));
    ui->tw_specialFunctionRegister->setItem(5, 0, new QTableWidgetItem("Status"));
    ui->tw_specialFunctionRegister->setItem(6, 0, new QTableWidgetItem("Option"));
    ui->tw_specialFunctionRegister->setItem(7, 0, new QTableWidgetItem("INTCON"));

    ui->tw_specialFunctionRegister->setItem(0, 1, new QTableWidgetItem("00"));
    ui->tw_specialFunctionRegister->setItem(1, 1, new QTableWidgetItem("00"));
    ui->tw_specialFunctionRegister->setItem(2, 1, new QTableWidgetItem("00"));
    ui->tw_specialFunctionRegister->setItem(3, 1, new QTableWidgetItem("00"));
    ui->tw_specialFunctionRegister->setItem(4, 1, new QTableWidgetItem("00"));
    ui->tw_specialFunctionRegister->setItem(5, 1, new QTableWidgetItem("00"));
    ui->tw_specialFunctionRegister->setItem(6, 1, new QTableWidgetItem("00"));
    ui->tw_specialFunctionRegister->setItem(7, 1, new QTableWidgetItem("00"));

    ui->tw_specialFunctionRegister->setItem(0, 2, new QTableWidgetItem("00000000"));
    ui->tw_specialFunctionRegister->setItem(1, 2, new QTableWidgetItem("00000000"));
    ui->tw_specialFunctionRegister->setItem(2, 2, new QTableWidgetItem("00000000"));
    ui->tw_specialFunctionRegister->setItem(3, 2, new QTableWidgetItem("00000000"));
    ui->tw_specialFunctionRegister->setItem(4, 2, new QTableWidgetItem("00000000"));
    ui->tw_specialFunctionRegister->setItem(5, 2, new QTableWidgetItem("00000000"));
    ui->tw_specialFunctionRegister->setItem(6, 2, new QTableWidgetItem("00000000"));
    ui->tw_specialFunctionRegister->setItem(7, 2, new QTableWidgetItem("00000000"));
	//refreshSFRWidget();
}

void MainWindow::slotRefreshSpeicher()
{
    for(int i=0; i < n_register; i++)
    {
       // cout << "Adresse: " << i << "-" << steuerwerk->readForGUI(0,i) << endl;
        ui->tw_speicher->item(i,1)->setText(convertIntToHexString(getIntFromFile(0,i)));
        ui->tw_speicher->item(i+n_register,1)->setText(convertIntToHexString(getIntFromFile(1 ,i)));

        ui->tw_speicher->item(i,2)->setText(convertIntToBinString(getIntFromFile(0,i)));
        ui->tw_speicher->item(i+n_register,2)->setText(convertIntToBinString(getIntFromFile(1,i)));
    }

    refreshSFRWidget();
}

/*
 *  W-Reg fehlt noch
 */
void MainWindow::refreshSFRWidget()
{
    ui->tw_specialFunctionRegister->item(0, 1)->setText(convertIntToHexString(getIntFromW()));
    ui->tw_specialFunctionRegister->item(1, 1)->setText(convertIntToHexString(getIntFromFile(0, 0x04)));
    ui->tw_specialFunctionRegister->item(2, 1)->setText(convertIntToHexString(getIntFromFile(0, 0x02)));
    ui->tw_specialFunctionRegister->item(3, 1)->setText(convertIntToHexString(getIntFromFile(0, 0x0A)));
    ui->tw_specialFunctionRegister->item(4, 1)->setText(convertIntToHexString(getIntFromPC()));
    ui->tw_specialFunctionRegister->item(5, 1)->setText(convertIntToHexString(getIntFromFile(0, 0x03)));
    ui->tw_specialFunctionRegister->item(6, 1)->setText(convertIntToHexString(getIntFromFile(1, 0x01)));
    ui->tw_specialFunctionRegister->item(7, 1)->setText(convertIntToHexString(getIntFromFile(0, 0x1B)));

    ui->tw_specialFunctionRegister->item(0, 2)->setText(convertIntToBinString(getIntFromW()));
    ui->tw_specialFunctionRegister->item(1, 2)->setText(convertIntToBinString(getIntFromFile(0, 0x04)));
    ui->tw_specialFunctionRegister->item(2, 2)->setText(convertIntToBinString(getIntFromFile(0, 0x02)));
    ui->tw_specialFunctionRegister->item(3, 2)->setText(convertIntToBinString(getIntFromFile(0, 0x0A)));
    ui->tw_specialFunctionRegister->item(4, 2)->setText(convertIntToBinString(getIntFromPC()));
    ui->tw_specialFunctionRegister->item(5, 2)->setText(convertIntToBinString(getIntFromFile(0, 0x03)));
    ui->tw_specialFunctionRegister->item(6, 2)->setText(convertIntToBinString(getIntFromFile(1, 0x01)));
    ui->tw_specialFunctionRegister->item(7, 2)->setText(convertIntToBinString(getIntFromFile(0, 0x1B)));
}

int MainWindow::getIntFromFile(int bank, int file)
{
    return steuerwerk->readForGUI(bank,file);
}

int MainWindow::getIntFromW()
{
    return steuerwerk->readWForGUI();
}

int MainWindow::getIntFromPC()
{
    return steuerwerk->getPCInt();
}

QString MainWindow::convertIntToBinString(int value)
{
   int mask = 0x0080; // entspricht 0b10000000
   QString sresult;

   for(int schleife = 0; schleife < 8;schleife++)
   {

       if(value & mask)
           sresult.append('1');
       else
           sresult.append('0');

       mask=mask>>1;
   }

   return sresult;
}
QString MainWindow::convertIntToHexString(int value)
{
   int mask = 0x00f0; // entspricht 0b11110000
   QString sresult;

   for(int schleife = 0; schleife < 2;schleife++)
   {

       int nibble=value & mask;
       if(0==schleife) nibble >>= 4;
       switch (nibble) {
       case 0:
           sresult.append('0');
           break;
       case 1:
           sresult.append('1');
           break;
       case 2:
           sresult.append('2');
           break;
       case 3:
           sresult.append('3');
           break;
       case 4:
           sresult.append('4');
           break;
       case 5:
           sresult.append('5');
           break;
       case 6:
           sresult.append('6');
           break;
       case 7:
           sresult.append('7');
           break;
       case 8:
           sresult.append('8');
           break;
       case 9:
           sresult.append('9');
           break;
       case 10:
           sresult.append('A');
           break;
       case 11:
           sresult.append('B');
           break;
       case 12:
           sresult.append('C');
           break;
       case 13:
           sresult.append('D');
           break;
       case 14:
           sresult.append('E');
           break;
       case 15:
           sresult.append('F');
           break;
       default:
           break;
       }
       mask=mask>>4;
   }

   return sresult;
}

// Mario ende

void MainWindow::setLineColorWhite(int linenumber)
{
    QColor white(255,255,255);
    ui->lw_lstFile->item(linenumber)->setBackgroundColor(white);
}

void MainWindow::setLineColorGreen(int linenumber)
{
    QColor green(0,255,0);
    ui->lw_lstFile->item(linenumber)->setBackgroundColor(green);
}

void MainWindow::setLineColorRed(int linenumber)
{
    QColor red(255,0,0);
    ui->lw_lstFile->item(linenumber)->setBackgroundColor(red);
}

void MainWindow::slotExecuteStep()
{
    steuerwerk->executeStep();  // nächsten Befehl auf den der PC zeigt ausführen
}

void MainWindow::gotoLineNumber(int linenumber)
{
    ui->lw_lstFile->setCurrentRow(linenumber);
    ui->lw_lstFile->clearSelection();
}

//Benedikt:
void MainWindow::on_lw_lstFile_doubleClicked(const QModelIndex &index)
{
	if(steuerwerk->toggleBreakpoint(index.row()+1))
	{
        setLineColorRed(index.row());
	}
	else
	{
        setLineColorWhite(index.row());

	}
    ui->lw_lstFile->clearSelection();
	//index.row()+1 entspeichter Textzeile aus der lst Datei
}

void MainWindow::slotResetClicked()
{
    if(steuerwerk->lstFile.empty())
        return;

	//aktuellen Befehl weiß färben
	if(steuerwerk->pc < steuerwerk->maschinencode.end())
		setLineColorWhite(steuerwerk->getCurrentLineNumber()-1);


	//Steuerwerk resetten
	steuerwerk->clearSteuerwerk();
    slotRefreshSpeicher();
    refreshSFRWidget();
    refreshRuntime();

	//ersten Befehl einfärben
    setLineColorGreen(steuerwerk->getCurrentLineNumber()-1);
    gotoLineNumber(steuerwerk->getCurrentLineNumber()-1);
}

void MainWindow::slotGoClicked()
{
    if(steuerwerk->isRunning)
    {//Der Simulator wird gestoppt
        steuerwerk->isRunning = false;
        ui->goButton->setText("Go");
        ui->resetButton->setEnabled(true);
        ui->pb_executeStep->setEnabled(true);
        ui->refresh_speicher->setEnabled(true);
        ui->pb_load->setEnabled(true);
        ui->selectFile_Button->setEnabled(true);
        ui->verzoegerung->setHidden(true);
        ui->label_verzoegerung->setHidden(true);

    }
    else
    {//Der Simulator wird gestartet
        steuerwerk->isRunning = true;
        ui->goButton->setText("Stop");
        ui->resetButton->setEnabled(false);
        ui->pb_executeStep->setEnabled(false);
        ui->refresh_speicher->setEnabled(false);
        ui->pb_load->setEnabled(false);
        ui->selectFile_Button->setEnabled(false);
        ui->verzoegerung->setHidden(false);
        ui->label_verzoegerung->setHidden(false);
        QThread* workerThread = new QThread(); // erzeuge Thread zur ausführung
        GoKlasse* worker = new GoKlasse(steuerwerk); // erzeuge Worker object
        connect(worker,SIGNAL(slotGoClicked()),SLOT(slotGoClicked()));

        //connect all signals from worker object
        connect(worker,SIGNAL(setLineColorWhite(int)),SLOT(setLineColorWhite(int)));
        connect(worker,SIGNAL(setLineColorGreen(int)),SLOT(setLineColorGreen(int)));
        connect(worker,SIGNAL(setLineColorRed(int)),SLOT(setLineColorRed(int)));
        connect(worker,SIGNAL(gotoLineNumber(int)),SLOT(gotoLineNumber(int)));
        connect(worker,SIGNAL(refreshSFRWidget()),SLOT(refreshSFRWidget()));
        connect(worker,SIGNAL(slotRefreshSpeicher()),SLOT(slotRefreshSpeicher()));
        connect(worker,SIGNAL(getVerzoegerung(int*)),SLOT(readVerzoegerung(int*)));
        connect(worker,SIGNAL(refreshRuntime()),SLOT(refreshRuntime()));

        //worker object in Threadkontext verschieben
        worker->moveToThread(workerThread);

        //arbeit starten
        workerThread->start();
        QMetaObject::invokeMethod(worker, "run", Qt::QueuedConnection);

    }
}

void MainWindow::readVerzoegerung(int* value)
{
    *value = ui->verzoegerung->value();
}

void MainWindow::on_frequency_valueChanged(int arg1)
{
    stringstream ss;
    ss << (double)4*(1/(double)arg1);
    ui->cycle->setText(QString::fromStdString(ss.str()));
}

void MainWindow::refreshRuntime(void)
{
    steuerwerk->setTimePerCycle(4*(1/(double)ui->frequency->value()));
    stringstream ss;
    ss << steuerwerk->computeRuntime();
    //cout << "Runtime: " << steuerwerk->computeRuntime() << endl;
    ui->runtime->setText(QString::fromStdString(ss.str()));
}
