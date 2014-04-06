#ifndef SELECTFILEDIALOG_H
#define SELECTFILEDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>

namespace Ui {
class selectFileDialog;
}

class selectFileDialog : public QDialog
{
    Q_OBJECT

public:
    QString acceptedPath;
    explicit selectFileDialog(QWidget *parent = 0);
    ~selectFileDialog();

private slots:
    void on_FolderTree_clicked(const QModelIndex &index);


    void on_FileList_clicked(const QModelIndex &index);

private:

    Ui::selectFileDialog *ui;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;
};

#endif // SELECTFILEDIALOG_H
