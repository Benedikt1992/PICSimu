#include "selectfiledialog.h"
#include "ui_selectfiledialog.h"

selectFileDialog::selectFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectFileDialog)
{
    ui->setupUi(this);
    QString sPath = "C:/";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirmodel->setRootPath(sPath);
    ui->FolderTree->setModel(dirmodel);

    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    filemodel->setRootPath(sPath);
    ui->FileList->setModel(filemodel);
}

selectFileDialog::~selectFileDialog()
{
    delete ui;
}

void selectFileDialog::on_FolderTree_clicked(const QModelIndex &index)
{
    QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
    ui->FileList->setRootIndex(filemodel->setRootPath(sPath));
}
