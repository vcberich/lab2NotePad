#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>

class QPlainTextEdit;

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr, QPlainTextEdit *textEdit = nullptr);
    ~ReplaceDialog();

private slots:
    void on_btnFindNext_clicked();
    void on_btnReplace_clicked();
    void on_btnFindNext_4_clicked();      // 全部替换
    void on_btCancel_clicked();

private:
    // 查找并选中下一个匹配项；quiet 为 true 时找不到不弹提示
    bool findNext(bool forward, bool quiet = false);
    bool replaceCurrent();                // 把当前选中的匹配项替换掉
    int replaceAll();                     // 全部替换，返回替换处数

    Ui::ReplaceDialog *ui;
    QPlainTextEdit *pTextEdit;
};

#endif // REPLACEDIALOG_H
