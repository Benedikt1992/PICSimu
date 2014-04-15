#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"
#include "codeline.h"
#include <iostream>
#include <QDialog>
#include <sstream>
#include <string>
#include <QFileDialog>

#define n_register 48

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

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
}

void MainWindow::slotRefreshSpeicher()
{
    for(int i=0; i < n_register; i++)
    {
       // cout << "Adresse: " << i << "-" << steuerwerk->readForGUI(0,i) << endl;
        ui->tw_speicher->item(i,1)->setText(convertFileToHexString(0,i));
        ui->tw_speicher->item(i+n_register,1)->setText(convertFileToHexString(1 ,i));

        ui->tw_speicher->item(i,2)->setText(convertFileToBinString(0,i));
        ui->tw_speicher->item(i+n_register,2)->setText(convertFileToBinString(1,i));
    }

    refreshSFRWidget();
}

/*
 *  W-Reg fehlt noch
 */
void MainWindow::refreshSFRWidget()
{
    cout << "SFR refresh()" << endl;
    //ui->tw_specialFunctionRegister->item(0, 1)->setText(convertFileToHexString()); getter für W-Reg implementieren
    ui->tw_specialFunctionRegister->item(1, 1)->setText(convertFileToHexString(0, 0x04));
    ui->tw_specialFunctionRegister->item(2, 1)->setText(convertFileToHexString(0, 0x02));
    ui->tw_specialFunctionRegister->item(3, 1)->setText(convertFileToHexString(0, 0x0A));
    ui->tw_specialFunctionRegister->item(4, 1)->setText("0000");
    ui->tw_specialFunctionRegister->item(5, 1)->setText(convertFileToHexString(0, 0x03));
    ui->tw_specialFunctionRegister->item(6, 1)->setText(convertFileToHexString(1, 0x01));
    ui->tw_specialFunctionRegister->item(7, 1)->setText(convertFileToHexString(0, 0x1B));

    //ui->tw_specialFunctionRegister->item(0, 2)->setText(convertFileToHexString()); getter für W-Reg implementieren
    ui->tw_specialFunctionRegister->item(1, 2)->setText(convertFileToBinString(0, 0x04));
    ui->tw_specialFunctionRegister->item(2, 2)->setText(convertFileToBinString(0, 0x02));
    ui->tw_specialFunctionRegister->item(3, 2)->setText(convertFileToBinString(0, 0x0A));
    ui->tw_specialFunctionRegister->item(4, 2)->setText("00000000");
    ui->tw_specialFunctionRegister->item(5, 2)->setText(convertFileToBinString(0, 0x03));
    ui->tw_specialFunctionRegister->item(6, 2)->setText(convertFileToBinString(1, 0x01));
    ui->tw_specialFunctionRegister->item(7, 2)->setText(convertFileToBinString(0, 0x1B));
}

QString MainWindow::getQStringFromFile(int bank, int file)
{

}

QString MainWindow::convertFileToBinString(int bank,int file)
{
   int value = steuerwerk->readForGUI(bank,file);
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
QString MainWindow::convertFileToHexString(int bank,int file)
{
   int value = steuerwerk->readForGUI(bank,file);
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
    setLineColorWhite(steuerwerk->getCurrentLineNumber()-1);


	//Steuerwerk resetten
	steuerwerk->clearSteuerwerk();
    slotRefreshSpeicher();
    refreshSFRWidget();

	//ersten Befehl einfärben
    setLineColorGreen(steuerwerk->getCurrentLineNumber()-1);
    gotoLineNumber(steuerwerk->getCurrentLineNumber()-1);
}
