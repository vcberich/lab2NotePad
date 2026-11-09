#include "serchdialog.h"
#include "ui_serchdialog.h"

SerchDialog::SerchDialog(QWidget *parent ,QPlainTextEdit * textEdit)
    : QDialog(parent)
    , ui(new Ui::SerchDialog)
{
    ui->setupUi(this);

    pTextEdit = textEdit;
}

SerchDialog::~SerchDialog()
{
    delete ui;
}

void SerchDialog::on_btnFindNext_clicked()
{
    QString target = ui->lineEdit->text();
    QString text = pTextEdit->toPlainText();
    QTextCursor c=pTextEdit->textCursor();
    int index=-1;

    if(ui->rdDown->isCheckable();


    index=text.IndexOf(target, c.position());
    if(index>=0){
        c.setPosition(index);
        c.setPosition(index +target.length(),QTextCursor::KeepAnchor);
        pTextEdit->setTextCursor(c);
    }




    else if(ui->rdUp->isChecked()){
    index=text.lastIndexOf(target, c.position()-text.length()-1);
    if(index>=0){
        c.setPosition(index +target.length());
        c.setPosition(index ,QTextCursor::KeepAnchor);
        pTextEdit->setTextCursor(c);
    }
    }


}
void SerchDialog::on_btCancel_clicked()
{
    accept();
}

