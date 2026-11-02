#ifndef SERCHDIALOG_H
#define SERCHDIALOG_H

#include <QDialog>

namespace Ui {
class SerchDialog;
}

class SerchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerchDialog(QWidget *parent = nullptr);
    ~SerchDialog();

private:
    Ui::SerchDialog *ui;
};

#endif // SERCHDIALOG_H
