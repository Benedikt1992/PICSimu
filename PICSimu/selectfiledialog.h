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
    QString lastClickedPath;
    explicit selectFileDialog(QWidget *parent = 0);
    ~selectFileDialog();

private slots:
    void on_FolderTree_clicked(const QModelIndex &index);




private:

    Ui::selectFileDialog *ui;
    QFileSystemModel *dirmodel;

};

#endif // SELECTFILEDIALOG_H
