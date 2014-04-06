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

    QFont font = QFont ("Courier");
    font.setStyleHint (QFont::Monospace);
    font.setPointSize (8);
    font.setFixedPitch (true);

    ui->lw_lstFile->setFont(font);

    connect(ui->pb_load, SIGNAL(clicked()), SLOT(slotLoadLstFile()));
    connect(ui->selectFile_Button, SIGNAL(clicked()),SLOT(load_FileDialog()));
    //connect(selectFileDialog.ui->buttonBox,SIGNAL(accepted()),this,SLOT(writePath()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotLoadLstFile()
{

    list<string> lst;
    Steuerwerk* pic = new Steuerwerk();



    Parser::auslesen(&lst, ui->le_filename->text().toStdString(), pic);



    for(list<string>::iterator it=lst.begin(); it!=lst.end(); it++)
    {
        QString qstr = QString::fromStdString(*it);
        ui->lw_lstFile->addItem(qstr);
    }
}

void MainWindow::load_FileDialog()
{
   if (FileDialog.exec()== QDialog::Accepted) //Blocking call!!! wird erst beendet, wenn das fenster geschlossen wird
   {
       ui->le_filename->clear();
       ui->le_filename->setText(FileDialog.acceptedPath);
       //Hier code wenn ok geklickt wurde
   }
}




