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

    if(steuerwerk->loadFile(ui->le_filename->text().toStdString()))  // auslesen erfolgreich?
    {
        ui->lw_lstFile->clear();    // LineWidget leeren

        // cout << "Ausgabe gestartet" << endl;

		for(list<QString>::iterator it=steuerwerk->lstFile.begin(); it!=steuerwerk->lstFile.end(); it++)
		{
			ui->lw_lstFile->addItem(*it);   // Code zeilenweise in ListWidget einfügen
		}

		//ersten Befehl einfärben

		int zeile = steuerwerk->pc->textzeile - 1;
		QColor green(0,255,0);
		ui->lw_lstFile->item(zeile)->setBackgroundColor(green);

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

void MainWindow::slotRefreshSpeicher()
{ //BB äußerst unschön gelöst. Verletzt sämtliche Konzeptgrenzen. Stelle dir vor der Speicher ist private. besser eine Methode speicherwer->getNumberOn(bank, adresse) und von dort methode von speicher aufrufen.
    int* bank0;
    int* bank1;

    bank0 = steuerwerk->getBank(0);
    bank1 = steuerwerk->getBank(1);

    if(bank0 == 0 || bank1 == 0)
        return;

    for(int i=0; i < n_register; i++)
    {
        ui->tw_speicher->item(i,1)->setText(QString::number(bank0[i], 16));
        ui->tw_speicher->item(i+n_register,1)->setText(QString::number(bank1[i], 16));

        ui->tw_speicher->item(i,2)->setText(QString::number(bank0[i], 2));
        ui->tw_speicher->item(i+n_register,2)->setText(QString::number(bank1[i], 2));
    }
}

// Mario ende

void MainWindow::slotExecuteStep() //BB generell Besser gesamte Funktionalität in Steuerwerk legen und von dort Mehtoden der GUI aufrufen (dafür haben wir ja das mainwindow attribut in steuerwerk). Spätestens bei run() ist das wahrscheinlich zwingend nötig, da die Methode in einer Schleife läuft.
{
    if(steuerwerk->programmEndeErreicht())
        return;
	//BB besser steuerwerk->getCurrentLine(). siehe auch weitere Vorkommen in Methode
    int zeile = steuerwerk->pc->textzeile - 1;  // Zeilennummer entspricht nicht der item-Nummer des ListWidgets!
    QColor white(255,255,255);
    ui->lw_lstFile->item(zeile)->setBackgroundColor(white);

    steuerwerk->executeStep();  // nächsten Befehl auf den der PC zeigt ausführen
    slotRefreshSpeicher();  // nicht notwendig es als Slot zu definieren

    if(steuerwerk->programmEndeErreicht())
        return;

    // nächsten Befehl in der GUI einfärben
    zeile = steuerwerk->pc->textzeile - 1;
    QColor green(0,255,0);
    ui->lw_lstFile->item(zeile)->setBackgroundColor(green);

    // zur aktuellen Stelle in der Dateiansicht springen
    ui->lw_lstFile->setCurrentRow(zeile);
    ui->lw_lstFile->clearSelection();
}

//Benedikt:
void MainWindow::on_lw_lstFile_doubleClicked(const QModelIndex &index)
{
	if(steuerwerk->toggleBreakpoint(index.row()+1))
	{
		QColor myColor(255,0,0);
		ui->lw_lstFile->item(index.row())->setBackgroundColor(myColor);
	}
	else
	{
		QColor myColor(255,255,255);
		ui->lw_lstFile->item(index.row())->setBackgroundColor(myColor);
	}

	//index.row()+1 entspeichter Textzeile aus der lst Datei
}
