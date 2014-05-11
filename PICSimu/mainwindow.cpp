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
#include <stdlib.h>

#define n_register 48
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

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
    connect(ui->tw_RA, SIGNAL(cellClicked(int,int)), SLOT(slotRAValueChanged(int,int)));
    connect(ui->tw_RB, SIGNAL(cellClicked(int,int)), SLOT(slotRBValueChanged(int,int)));
	connect(ui->tw_RA, SIGNAL(cellDoubleClicked(int,int)),SLOT(slotPortDoubleClicked(int,int)));
	connect(ui->tw_RB, SIGNAL(cellDoubleClicked(int,int)),SLOT(slotPortDoubleClicked(int,int)));
    connect(ui->tw_speicher, SIGNAL(cellChanged(int,int)), SLOT(slotSpeicherChanged(int, int)));



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


    ui->tw_specialFunctionRegister->setRowCount(8);
    ui->tw_specialFunctionRegister->setColumnCount(3);
    ui->tw_specialFunctionRegister->verticalHeader()->setVisible(false);

    ui->tw_specialFunctionRegister->setColumnWidth(0,60);
    ui->tw_specialFunctionRegister->setColumnWidth(1,35);
    ui->tw_specialFunctionRegister->setColumnWidth(2,65);

    ui->tw_specialFunctionRegister->setHorizontalHeaderItem(0,new QTableWidgetItem("Name"));
    ui->tw_specialFunctionRegister->setHorizontalHeaderItem(1,new QTableWidgetItem("Hex"));
    ui->tw_specialFunctionRegister->setHorizontalHeaderItem(2,new QTableWidgetItem("Bin"));
    ui->tw_specialFunctionRegister->setFont(font);

    initializeSFRWidget();

    //Verzögerungs spinbox initialisieren
    ui->verzoegerung->setMaximum(250);
    ui->verzoegerung->setValue(100);
    ui->verzoegerung->setMinimum(50);
    ui->verzoegerung->setHidden(true);
    ui->label_verzoegerung->setHidden(true);

    //Initialisieren Frequenz
    ui->frequency->setValue(4);
	ui->frequency->setMinimum(0.1);
    ui->frequency->setMaximum(10);
	ui->frequency->setDecimals(4);
    ui->cycle->setText(QString::fromStdString("1"));
    ui->runtime->setText(QString::fromStdString("0"));

	ui->WDTcheckBox->setCheckState(Qt::Checked);

    // RA initialisieren
    ui->tw_RA->setRowCount(2);
    ui->tw_RA->setColumnCount(8);
    ui->tw_RA->setFont(font);

    for(int i = 0; i < 8; i++)
    {
        ui->tw_RA->setColumnWidth(i,17);
        ui->tw_RA->setHorizontalHeaderItem(i,new QTableWidgetItem(QString::number(7-i)));
        ui->tw_RA->setItem(0,i,new QTableWidgetItem("i"));
        ui->tw_RA->setItem(1,i,new QTableWidgetItem("0"));
    }

    ui->tw_RA->setVerticalHeaderItem(0,new QTableWidgetItem("Tris"));
    ui->tw_RA->setVerticalHeaderItem(1,new QTableWidgetItem("Pins"));
    ui->tw_RA->verticalHeader()->sizeHint().width();

    // RB initialisieren
    ui->tw_RB->setRowCount(2);
    ui->tw_RB->setColumnCount(8);
    ui->tw_RB->setFont(font);

    for(int i = 0; i < 8; i++)
    {
        ui->tw_RB->setColumnWidth(i,17);
        ui->tw_RB->setHorizontalHeaderItem(i,new QTableWidgetItem(QString::number(7-i)));
        ui->tw_RB->setItem(0,i,new QTableWidgetItem("i"));
        ui->tw_RB->setItem(1,i,new QTableWidgetItem("0"));
    }

    ui->tw_RB->setVerticalHeaderItem(0,new QTableWidgetItem("Tris"));
    ui->tw_RB->setVerticalHeaderItem(1,new QTableWidgetItem("Pins"));


}

MainWindow::~MainWindow()
{
    steuerwerk->alu->speicher.terminateEEPROM = true;
    delete ui;
}

void MainWindow::connectSteuerwerk(Steuerwerk* steuerwerk)
{
    this->steuerwerk = steuerwerk;
}

void MainWindow::slotLoadLstFile()
{
	// evtl. vorhandene Inhalte löschen
	steuerwerk->clearProgrammspeicher();
	steuerwerk->clearSteuerwerk();
	ui->lw_lstFile->clear();    // LineWidget leeren
	slotRefreshSpeicher();


    if(steuerwerk->loadFile(ui->le_filename->text().toStdString()))  // auslesen erfolgreich?
    {
        ui->lw_lstFile->clear();    // LineWidget leeren

		for(list<QString>::iterator it=steuerwerk->lstFile.begin(); it!=steuerwerk->lstFile.end(); it++)
		{
			ui->lw_lstFile->addItem(*it);   // Code zeilenweise in ListWidget einfügen
		}

		//ersten Befehl einfärben
        setLineColorGreen(steuerwerk->getCurrentLineNumber()-1);

    }
    else
        ui->lw_lstFile->addItem("File does not exist!");

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
    // cellChanged soll nicht beim Refresh aufgerufen werden
    disconnect(ui->tw_speicher, SIGNAL(cellChanged(int,int)), this, SLOT(slotSpeicherChanged(int, int)));

    for(int i=0; i < n_register; i++)
    {
        ui->tw_speicher->item(i,1)->setText(convertIntToHexString(getIntFromFile(0,i)));
        ui->tw_speicher->item(i+n_register,1)->setText(convertIntToHexString(getIntFromFile(1 ,i)));

        ui->tw_speicher->item(i,2)->setText(convertIntToBinString(getIntFromFile(0,i)));
        ui->tw_speicher->item(i+n_register,2)->setText(convertIntToBinString(getIntFromFile(1,i)));
    }



    // notwendiger Reconnect!
    connect(ui->tw_speicher, SIGNAL(cellChanged(int,int)), SLOT(slotSpeicherChanged(int, int)));
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
    ui->tw_specialFunctionRegister->item(7, 1)->setText(convertIntToHexString(getIntFromFile(0, 0x0B)));

    ui->tw_specialFunctionRegister->item(0, 2)->setText(convertIntToBinString(getIntFromW()));
    ui->tw_specialFunctionRegister->item(1, 2)->setText(convertIntToBinString(getIntFromFile(0, 0x04)));
    ui->tw_specialFunctionRegister->item(2, 2)->setText(convertIntToBinString(getIntFromFile(0, 0x02)));
    ui->tw_specialFunctionRegister->item(3, 2)->setText(convertIntToBinString(getIntFromFile(0, 0x0A)));
    ui->tw_specialFunctionRegister->item(4, 2)->setText(convertIntToBinString(getIntFromPC()));
    ui->tw_specialFunctionRegister->item(5, 2)->setText(convertIntToBinString(getIntFromFile(0, 0x03)));
    ui->tw_specialFunctionRegister->item(6, 2)->setText(convertIntToBinString(getIntFromFile(1, 0x01)));
    ui->tw_specialFunctionRegister->item(7, 2)->setText(convertIntToBinString(getIntFromFile(0, 0x0B)));
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

int MainWindow::getFirstIntFromStack()
{
    return steuerwerk->getStackInt();
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


void MainWindow::on_lw_lstFile_doubleClicked(const QModelIndex &index)
{
    // wechsle den Breakpint und färbe Zeile entsprechend
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

	setLineColorGreen(steuerwerk->getCurrentLineNumber()-1);
}

void MainWindow::refreshStorageElements()
{
    slotRefreshSpeicher();
    refreshSFRWidget();
    refreshRuntime();
    refreshStack();
    refreshRA();
    refreshRB();
}

void MainWindow::slotResetClicked()
{
    if(steuerwerk->lstFile.empty())
        return;

    //aktuellen Befehl entfärben, je nachdem ob breakpoint oder nicht
	if(steuerwerk->pc < steuerwerk->maschinencode.end())
	{
		if(steuerwerk->pc->breakpoint)
			setLineColorRed(steuerwerk->getCurrentLineNumber()-1);
		else
			setLineColorWhite(steuerwerk->getCurrentLineNumber()-1);
	}

	//Steuerwerk resetten
	steuerwerk->clearSteuerwerk();
    refreshStorageElements();

	//ersten Befehl einfärben
    setLineColorGreen(steuerwerk->getCurrentLineNumber()-1);
    gotoLineNumber(steuerwerk->getCurrentLineNumber()-1);

	//WDT aktiviert?
	if(ui->WDTcheckBox->isChecked())
	{
		steuerwerk->alu->speicher.write2007(steuerwerk->alu->speicher.read2007()|0x0004);
	}
	else
	{
		steuerwerk->alu->speicher.write2007(steuerwerk->alu->speicher.read2007()&0xfffb);
	}
}

void MainWindow::slotRAValueChanged(int row, int column)
{
    if(steuerwerk == NULL)
        return;

    if(row == 0)
        return;

    int currentValue = ui->tw_RA->item(row, column)->text().toInt();

	//der Wert muss geändert werden, bit1-31 muss 0 sein!
	int newValue = (~currentValue) &1;

    ui->tw_RA->item(row, column)->setText(QString::number(newValue));

    ui->tw_RA->setCurrentCell(-1,-1);
    ui->tw_RA->clearSelection();

    int value = steuerwerk->readForGUI(0, 0x05);
    int bit = 7 - column;

    if(newValue == 0)
        value &= ~(1 << bit);
    else
        value |= 1 << bit;

    // nur schreiben, wenn im TrisRegister der PIN als Input definiert ist
    int trisAValue = steuerwerk->readForGUI(1, 0x05);
    bool isInput = CHECK_BIT(trisAValue, bit);

    if(isInput)
        steuerwerk->writeRAFromGUI(value);

    refreshStorageElements();
}

void MainWindow::setIntf()
{
    int intcon = steuerwerk->readForGUI(0, 0x0B);
    intcon |= 1 << 1;
    steuerwerk->alu->speicher.writeOnBank(0, 0x0B, intcon);
}

void MainWindow::setRbif()
{
    int intcon = steuerwerk->readForGUI(0, 0x0B);
    intcon |= 1;
    steuerwerk->alu->speicher.writeOnBank(0, 0x0B, intcon);
}

void MainWindow::slotRBValueChanged(int row, int column)
{
    if(steuerwerk == NULL)
        return;

    if(row == 0)
        return;

    int currentValue = ui->tw_RB->item(row, column)->text().toInt();
    int newValue = (~currentValue) & 1;

    ui->tw_RB->item(row, column)->setText(QString::number(newValue));

    ui->tw_RB->setCurrentCell(-1,-1);
    ui->tw_RB->clearSelection();

    // RB0/INT - Interrupt
    // Edge-Select auswählen
    int option = steuerwerk->alu->speicher.readOnBank(1, 0x01);
    bool intedg = CHECK_BIT(option, 6);

    int value = steuerwerk->readForGUI(0, 0x06);
    int bit = 7 - column;

    bool newRB0Value = CHECK_BIT(value, 0);

    if(bit == 0)
    {
        if(intedg)  // positive Flanke
        {
            if(!lastRB0Value && newRB0Value)
                setIntf();
        }
        else        // negative Flanke
        {
            if(lastRB0Value && !newRB0Value)
                setIntf();
        }
    }

    lastRB0Value = newRB0Value;


    // RB7:RB4 - Interrupt ausgelöst
    int trisbValue = steuerwerk->readForGUI(1, 0x06);

    if(bit >= 4 && bit <= 7)
    {
        bool isInput = CHECK_BIT(trisbValue, bit);

        if(isInput)
            setRbif();
    }

    if(newValue == 0)
        value &= ~(1 << bit);
    else
        value |= 1 << bit;

    // nur schreiben, wenn im TrisRegister der PIN als Input definiert ist
    int trisBValue = steuerwerk->readForGUI(1, 0x06);
    bool isInput = CHECK_BIT(trisBValue, bit);

    if(isInput)
        steuerwerk->writeRBFromGUI(value);

    refreshStorageElements();
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
        connect(worker,SIGNAL(refreshStack()),SLOT(refreshStack()));

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

void MainWindow::on_frequency_valueChanged(double arg1)
{//Berechne die Zeit pro Zyklus neu
    stringstream ss;
    ss << (double)4*(1/(double)arg1);
    ui->cycle->setText(QString::fromStdString(ss.str()));
}

void MainWindow::refreshRuntime(void)
{
    steuerwerk->setTimePerCycle(4*(1/(double)ui->frequency->value()));
    stringstream ss;
    ss << steuerwerk->computeRuntime();
    ui->runtime->setText(QString::fromStdString(ss.str()));
}

void MainWindow::on_clearRuntime_clicked()
{
    steuerwerk->clearRuntime();
    refreshRuntime();
}

void MainWindow::refreshStack()
{
    int elements = ui->lw_stack->count();
    int stackElements = steuerwerk->picStack.size();

    if(stackElements == 0)
    {
        ui->lw_stack->clear();
        return;
    }

    if(elements == stackElements)
        return;

    if(elements > stackElements)
    {
        ui->lw_stack->takeItem(0);
        return;
    }

    if(stackElements > elements)
    {
        QString newStackValue = convertIntToHexString(getFirstIntFromStack());

        ui->lw_stack->addItem(newStackValue);
    }
}

void MainWindow::slotPortDoubleClicked(int /*unused*/, int /*unused*/)
{
	//TODO Textmarkierung entfernen
	ui->tw_RA->setCurrentCell(-1,-1);
	ui->tw_RA->clearSelection();
	ui->tw_RB->setCurrentCell(-1,-1);
	ui->tw_RB->clearSelection();
}

void MainWindow::slotSpeicherChanged(int row, int column)
{
    /*
     *  NULL-Referenzen abfangen
     */
    if(steuerwerk == NULL)
        return;

    if(steuerwerk->alu == NULL)
        return;

    // wenn Speicheradresse oder BIN-Wert manuell geändert wurde
    // => alten Wert wiederherstellen
    if(column == 0 || column == 2)
    {
        restoreOldValue(row, column);
        return;
    }

    int bank;

    bool ok;    // valider HEX-Wert eingelesen?
    int newValue = ui->tw_speicher->item(row,column)->text().toInt(&ok, 16);


    if(row <= 0x2F)
        bank = 0;
    else
        bank = 1;

    // Wert nur in den Speicher schreiben, wenn valider HEX-Wert eingelesen wurde
    if(ok)
        steuerwerk->alu->speicher.writeOnBank(bank, row % 0x30, newValue);

    // Wert aus dem Speicher auslesen und ausgeben
    // für den Fall, dass zuvor kein korrekter Wert eingegeben wurde
    refreshStorageElements();
}

void MainWindow::restoreOldValue(int row, int column)
{
    if(steuerwerk == NULL)
        return;

    if(column == 0)
    {
        if(row <= 0x2F)
            ui->tw_speicher->item(row, column)->setText(convertIntToHexString(row));
        else if(row > 0x2F)
            ui->tw_speicher->item(row, column)->setText(convertIntToHexString(row+0x80-0x2F-1));
    }
    else if(column == 2)
    {
        if(row <= 0x2F)
            ui->tw_speicher->item(row, column)->setText(convertIntToBinString(getIntFromFile(0,row)));
        else if(row > 0x2F)
            ui->tw_speicher->item(row, column)->setText(convertIntToBinString(getIntFromFile(1,row % 0x2F)));
    }
}

void MainWindow::refreshRA()
{
    if(steuerwerk == NULL)
        return;

    int valueRA = steuerwerk->readForGUI(0, 0x05);

    for(int bit=0; bit < 8; bit++)
    {
        int pinValue = 0;

        if(CHECK_BIT(valueRA, bit))
            pinValue = 1;

        ui->tw_RA->item(1, 7-bit)->setText(QString::number(pinValue));
    }

    int trisA = steuerwerk->readForGUI(1, 0x05);
    for(int bit=0; bit < 8; bit++)
    {
        QString pinValue;

        if(CHECK_BIT(trisA, bit))
            pinValue = "i";
        else
            pinValue = "o";

        ui->tw_RA->item(0, 7-bit)->setText(pinValue);
    }
}

void MainWindow::refreshRB()
{
    if(steuerwerk == NULL)
        return;

    int valueRB = steuerwerk->readForGUI(0, 0x06);

    for(int bit=0; bit < 8; bit++)
    {
        int pinValue = 0;

        if( 0 < CHECK_BIT(valueRB, bit))
            pinValue = 1;

        ui->tw_RB->item(1, 7-bit)->setText(QString::number(pinValue));
    }

    int trisB = steuerwerk->readForGUI(1, 0x06);
    for(int bit=0; bit < 8; bit++)
    {
        QString pinValue;

        if(CHECK_BIT(trisB, bit))
            pinValue = "i";
        else
            pinValue = "o";

        ui->tw_RB->item(0, 7-bit)->setText(pinValue);
    }
}
