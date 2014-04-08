#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_selectfiledialog.h"
#include "selectfiledialog.h"
#include "parser.h"
#include <iostream>
#include <QDialog>
#include <sstream>
#include <string>

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
    connect(ui->refresh_speicher, SIGNAL(clicked()), SLOT(slotRefreshSpeicher())); // Mario

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

    if(steuerwerk->loadFile(ui->le_filename->text().toStdString()))  // auslesen erfolgreich?
    {
        ui->lw_lstFile->clear();    // LineWidget leeren

        // cout << "Ausgabe gestartet" << endl;

		for(list<QString>::iterator it=steuerwerk->lstFile.begin(); it!=steuerwerk->lstFile.end(); it++)
		{
			ui->lw_lstFile->addItem(*it);   // Code zeilenweise in ListWidget einfügen
		}

    }
    else
        ui->lw_lstFile->addItem("File does not exist!");
    // cout << "slotLoadLstFile() beendet" << endl;
}

void MainWindow::slotLoad_FileDialog()
{
   if (FileDialog.exec()== QDialog::Accepted) //Blocking call!!! wird erst beendet, wenn das fenster geschlossen wird
   {
       ui->le_filename->clear();
       ui->le_filename->setText(FileDialog.lastClickedPath);
   }
}

// Mario
void MainWindow::slotRefreshSpeicher()
{
    int* bank0;
    int* bank1;

    bank0 = steuerwerk->getBank(0);
    bank1 = steuerwerk->getBank(1);

    if(bank0 == 0 || bank1 == 0)
        return;

    for(int i=0; i < n_register; i++)
    {
        ui->tw_speicher->setItem(i, 0, new QTableWidgetItem(QString::number(i,16)));
        ui->tw_speicher->setItem(i+n_register, 0, new QTableWidgetItem(QString::number(i+128,16)));

        ui->tw_speicher->setItem(i, 1, new QTableWidgetItem(QString::number(bank0[i], 16)));
        ui->tw_speicher->setItem(i+n_register, 1, new QTableWidgetItem(QString::number(bank1[i], 16)));

        ui->tw_speicher->setItem(i, 2, new QTableWidgetItem(QString::number(bank0[i], 2)));
        ui->tw_speicher->setItem(i+n_register, 2, new QTableWidgetItem(QString::number(bank1[i], 2)));

        ui->tw_speicher->item(i,2)->setTextAlignment(Qt::AlignRight);
        ui->tw_speicher->item(i+n_register,2)->setTextAlignment(Qt::AlignRight);
    }
}
// Mario ende


//Benedikt:
void MainWindow::on_lw_lstFile_doubleClicked(const QModelIndex &index)
{
	QColor myColor(255,0,0);
	ui->lw_lstFile->item(index.row())->setBackgroundColor(myColor);
	//index.row()+1 entspeichter Textzeile aus der lst Datei
}
