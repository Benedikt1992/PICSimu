#ifndef SELECTFILEDIALOG_H
#define SELECTFILEDIALOG_H

#include <QDialog>

namespace Ui {
class selectFileDialog;
}

class selectFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit selectFileDialog(QWidget *parent = 0);
    ~selectFileDialog();

private:
    Ui::selectFileDialog *ui;
};

#endif // SELECTFILEDIALOG_H
