#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void setLineColorWhite(int linenumber);
    void setLineColorGreen(int linenumber);
    void gotoLineNumber(int linenumber);

private:
    Ui::MainWindow *ui;
    void initializeSpeicherWidget(void);
private slots:
    void slotLoadLstFile(void);
    void slotLoad_FileDialog(void);
    void slotExecuteStep(void);
	void slotResetClicked(void);

	//Benedikt:
	void on_lw_lstFile_doubleClicked(const QModelIndex &index);
public slots:
    void slotRefreshSpeicher(void);

};

#endif // MAINWINDOW_H
