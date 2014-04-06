#include "selectfiledialog.h"
#include "ui_selectfiledialog.h"

selectFileDialog::selectFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectFileDialog)
{
    ui->setupUi(this);
}

selectFileDialog::~selectFileDialog()
{
    delete ui;
}
