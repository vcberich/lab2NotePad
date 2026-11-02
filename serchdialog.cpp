#include "serchdialog.h"
#include "ui_serchdialog.h"

SerchDialog::SerchDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SerchDialog)
{
    ui->setupUi(this);
}

SerchDialog::~SerchDialog()
{
    delete ui;
}
