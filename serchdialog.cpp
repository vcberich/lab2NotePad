#include "serchdialog.h"
#include "ui_serchdialog.h"

#include <QMessageBox>
#include <QTextCursor>

SerchDialog::SerchDialog(QWidget *parent, QPlainTextEdit *textEdit)
    : QDialog(parent)
    , ui(new Ui::SerchDialog)
    , pTextEdit(textEdit)
{
    ui->setupUi(this);
    ui->rdDown->setChecked(true);      // 默认向下查找

    if (!pTextEdit)
        setEnabled(false);
}

SerchDialog::~SerchDialog()
{
    delete ui;
}

// 核心查找：从当前光标（或选区边界）继续找，找到就选中并返回 true
bool SerchDialog::findNext(bool forward)
{
    if (!pTextEdit)
        return false;

    const QString target = ui->lineEdit->text();
    if (target.isEmpty())
    {
        QMessageBox::information(this, tr("查找"), tr("请输入要查找的内容"));
        return false;
    }

    const Qt::CaseSensitivity cs = ui->checkBox->isChecked() ? Qt::CaseSensitive
                                                             : Qt::CaseInsensitive;
    const QString text = pTextEdit->toPlainText();
    const QTextCursor cursor = pTextEdit->textCursor();

    int index = -1;
    if (forward)
    {
        // 从选区末尾之后开始找，实现“连续查找下一个”
        const int from = cursor.hasSelection() ? cursor.selectionEnd() : cursor.position();
        index = text.indexOf(target, from, cs);
        if (index < 0)                       // 到文末了，回到开头再找一次
            index = text.indexOf(target, 0, cs);
    }
    else
    {
        const int from = cursor.hasSelection() ? cursor.selectionStart() : cursor.position();
        index = text.lastIndexOf(target, from - 1, cs);
        if (index < 0)                       // 到文首了，从末尾再找一次
            index = text.lastIndexOf(target, text.length(), cs);
    }

    if (index < 0)
    {
        QMessageBox::information(this, tr("查找"), tr("未找到“%1”").arg(target));
        return false;
    }

    QTextCursor found = pTextEdit->textCursor();
    found.setPosition(index);
    found.setPosition(index + target.length(), QTextCursor::KeepAnchor);
    pTextEdit->setTextCursor(found);
    pTextEdit->ensureCursorVisible();
    return true;
}

void SerchDialog::on_btnFindNext_clicked()
{
    findNext(!ui->rdUp->isChecked());
}

void SerchDialog::on_btCancel_clicked()
{
    accept();
}
