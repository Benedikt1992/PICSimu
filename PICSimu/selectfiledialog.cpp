#include "selectfiledialog.h"
#include "ui_selectfiledialog.h"
#include "mainwindow.h"
#include <iostream>

selectFileDialog::selectFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectFileDialog)
{
    ui->setupUi(this);
    QString sPath = "C:/";
    dirmodel = new QFileSystemModel(this);
	dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    dirmodel->setRootPath(sPath);
    ui->FolderTree->setModel(dirmodel);


}

selectFileDialog::~selectFileDialog()
{
    delete ui;
}

void selectFileDialog::on_FolderTree_clicked(const QModelIndex &index)
{
    QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
	lastClickedPath = sPath;
}







