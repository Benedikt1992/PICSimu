#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "selectfiledialog.h"
//#include "steuerwerk.h"

namespace Ui {
class MainWindow;
}

class Steuerwerk;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Steuerwerk* steuerwerk;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void connectSteuerwerk(Steuerwerk*);

private:
    Ui::MainWindow *ui;
    selectFileDialog FileDialog;
private slots:
    void slotLoadLstFile(void);
    void slotLoad_FileDialog(void);
	//Benedikt:
	void on_lw_lstFile_doubleClicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
