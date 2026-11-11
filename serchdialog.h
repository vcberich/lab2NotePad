#ifndef SERCHDIALOG_H
#define SERCHDIALOG_H

#include <QDialog>
#include<QPlainTextEdit>

namespace Ui {
class SerchDialog;
}

class SerchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerchDialog(QWidget *parent = nullptr,QPlainTextEdit * textEdit=nullptr);
    ~SerchDialog();

private slots:
    void on_btnFindNext_clicked();

    void on_btCancel_clicked();

private:
    // 从当前光标位置继续查找，forward 为 true 表示向下查找
    bool findNext(bool forward);

    Ui::SerchDialog *ui;

    QPlainTextEdit *pTextEdit;
};

#endif // SERCHDIALOG_H
