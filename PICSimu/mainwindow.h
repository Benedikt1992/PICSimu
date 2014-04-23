#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


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



private:
    Ui::MainWindow *ui;
    void initializeSpeicherWidget(void);
    void initializeSFRWidget(void);
    QString convertIntToBinString(int value);
    QString convertIntToHexString(int value);
    int getIntFromFile(int, int);
    int getIntFromW();
    int getIntFromPC();
    int getFirstIntFromStack();
private slots:
    void slotLoadLstFile(void);
    void slotLoad_FileDialog(void);
    void slotExecuteStep(void);
	void slotResetClicked(void);


	//Benedikt:
	void on_lw_lstFile_doubleClicked(const QModelIndex &index);
    void on_frequency_valueChanged(int arg1);

    void on_clearRuntime_clicked();

public slots:
    void connectSteuerwerk(Steuerwerk*);
    void slotRefreshSpeicher(void);
    void slotGoClicked(void);

    //werden durch GoThread ausgelöst:
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
