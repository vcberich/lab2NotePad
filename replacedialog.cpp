#include "replacedialog.h"
#include "ui_replacedialog.h"

#include <QMessageBox>
#include <QPlainTextEdit>
#include <QTextCursor>
#include <QTextDocument>

namespace {
QTextDocument::FindFlags findFlags(bool caseSensitive, bool forward)
{
    QTextDocument::FindFlags flags;
    if (caseSensitive)
        flags |= QTextDocument::FindCaseSensitively;
    if (!forward)
        flags |= QTextDocument::FindBackward;
    return flags;
}
}

ReplaceDialog::ReplaceDialog(QWidget *parent, QPlainTextEdit *textEdit)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
    , pTextEdit(textEdit)
{
    ui->setupUi(this);
    ui->rdDown->setChecked(true);      // 默认向下

    if (!pTextEdit)
        setEnabled(false);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

bool ReplaceDialog::findNext(bool forward)
{
    if (!pTextEdit)
        return false;

    const QString target = ui->lineEdit->text();
    if (target.isEmpty())
    {
        QMessageBox::information(this, tr("替换"), tr("请输入要查找的内容"));
        return false;
    }

    const QTextDocument::FindFlags flags = findFlags(ui->checkBox->isChecked(), forward);
    QTextCursor found = pTextEdit->document()->find(target, pTextEdit->textCursor(), flags);

    if (found.isNull())            // 到边界了，绕到另一端再找一次
    {
        QTextCursor start = pTextEdit->textCursor();
        start.movePosition(forward ? QTextCursor::Start : QTextCursor::End);
        found = pTextEdit->document()->find(target, start, flags);
    }

    if (found.isNull())
    {
        QMessageBox::information(this, tr("替换"), tr("未找到“%1”").arg(target));
        return false;
    }

    pTextEdit->setTextCursor(found);
    pTextEdit->ensureCursorVisible();
    return true;
}

bool ReplaceDialog::replaceCurrent()
{
    if (!pTextEdit)
        return false;

    const QString target = ui->lineEdit->text();
    if (target.isEmpty())
        return false;

    QTextCursor cursor = pTextEdit->textCursor();
    if (!cursor.hasSelection())
        return false;

    const QString selected = cursor.selectedText();
    const bool same = ui->checkBox->isChecked()
                          ? (selected == target)
                          : (selected.compare(target, Qt::CaseInsensitive) == 0);
    if (!same)
        return false;

    cursor.insertText(ui->lineEdit_2->text());
    return true;
}

int ReplaceDialog::replaceAll()
{
    if (!pTextEdit)
        return 0;

    const QString target = ui->lineEdit->text();
    if (target.isEmpty())
        return 0;

    const QString replacement = ui->lineEdit_2->text();
    const QTextDocument::FindFlags flags = findFlags(ui->checkBox->isChecked(), true);

    QTextCursor cursor(pTextEdit->document());
    cursor.beginEditBlock();       // 整体算一次编辑，撤销时一次退回

    int count = 0;
    while (true)
    {
        QTextCursor found = pTextEdit->document()->find(target, cursor, flags);
        if (found.isNull())
            break;
        found.insertText(replacement);
        cursor = found;            // 光标已停在替换文本之后，继续往后找
        ++count;
    }

    cursor.endEditBlock();
    return count;
}

void ReplaceDialog::on_btnFindNext_clicked()
{
    findNext(!ui->rdUp->isChecked());
}

void ReplaceDialog::on_btnReplace_clicked()
{
    if (!replaceCurrent())
    {
        // 当前没选中匹配项，先找下一个，找到再替换
        if (!findNext(!ui->rdUp->isChecked()))
            return;
        if (!replaceCurrent())
            return;
    }

    findNext(!ui->rdUp->isChecked());   // 替换完成后跳到下一个匹配项
}

void ReplaceDialog::on_btnFindNext_4_clicked()
{
    const int count = replaceAll();
    if (count == 0)
        QMessageBox::information(this, tr("替换"), tr("未找到“%1”").arg(ui->lineEdit->text()));
    else
        QMessageBox::information(this, tr("替换"), tr("共替换 %1 处").arg(count));
}

void ReplaceDialog::on_btCancel_clicked()
{
    accept();
}
