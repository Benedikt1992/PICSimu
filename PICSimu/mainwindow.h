#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTableWidgedItem;

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



    void setIntf();
    void setRbif();

private:
    Ui::MainWindow *ui;
    void initializeSpeicherWidget(void);
    void initializeSFRWidget(void);
    QString convertIntToBinString(int value);
    QString convertIntToHexString(int value);
    int getIntFromFile(int bank, int file);
    int getIntFromW();
    int getIntFromPC();
    int getFirstIntFromStack();

    void refreshRA();
    void refreshRB();

    void restoreOldValue(int row, int column);

    void refreshFlagAndPc(void);

    bool lastRB0Value;
private slots:
    void slotLoadLstFile(void);
    void slotLoad_FileDialog(void);
    void slotExecuteStep(void);
	void slotResetClicked(void);
    void slotRAValueChanged(int, int);
    void slotRBValueChanged(int row, int column);
    void slotSpeicherChanged(int row, int column);
    void showHelp();



	//Benedikt:
	void on_lw_lstFile_doubleClicked(const QModelIndex &index);
	//void on_frequency_valueChanged(int arg1);

    void on_clearRuntime_clicked();

	void on_frequency_valueChanged(double arg1);

public slots:
    //werden durch Steuerwerk/GoKlasse/SleepKlasse ausgelöst:
    void connectSteuerwerk(Steuerwerk*);
    void slotRefreshSpeicher(void);
    void slotGoClicked(void);
    void refreshStorageElements();


    void setLineColorWhite(int linenumber);
    void setLineColorGreen(int linenumber);
    void setLineColorRed(int linenumber);
    void gotoLineNumber(int linenumber);
    void refreshSFRWidget(void);
    void readVerzoegerung(int*);
    void refreshRuntime(void);
    void refreshStack(void);
};

#endif // MAINWINDOW_H
