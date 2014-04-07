#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_selectfiledialog.h"
#include "selectfiledialog.h"
#include "parser.h"
#include <iostream>
#include <QDialog>

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotLoadLstFile()
{
    ui->lw_lstFile->clear();    // LineWidget leeren

    list<string> lst;
    Steuerwerk* pic = new Steuerwerk();

    if(Parser::auslesen(&lst, ui->le_filename->text().toStdString(), pic))  // auslesen erfolgreich?
        for(list<string>::iterator it=lst.begin(); it!=lst.end(); it++)
            ui->lw_lstFile->addItem(QString::fromStdString(*it));   // Code zeilenweise in ListWidget einfügen
    else
        ui->lw_lstFile->addItem("File does not exist!");
}

void MainWindow::slotLoad_FileDialog()
{
   if (FileDialog.exec()== QDialog::Accepted) //Blocking call!!! wird erst beendet, wenn das fenster geschlossen wird
   {
       ui->le_filename->clear();
       ui->le_filename->setText(FileDialog.acceptedPath);
       //Hier code wenn ok geklickt wurde
   }
}




