#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "selectfiledialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    selectFileDialog FileDialog;
private slots:
    void slotLoadLstFile(void);
    void slotLoad_FileDialog(void);
};

#endif // MAINWINDOW_H
