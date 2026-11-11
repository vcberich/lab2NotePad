#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "aboutdialog.h"
#include "serchdialog.h"
#include"replacedialog.h"
#include <qfiledialog.h>
#include<QMessageBox>
#include<QTextStream>
#include<QColorDialog>
#include<qfontdialog.h>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textChanged = false;

    statusLabel.setMaximumWidth(150);
    statusLabel.setText("length: " + QString::number(0) + "    lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);



    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("Ln: " + QString::number(0) + "    Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);


    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("author is TanZhian"));
    ui->statusbar->addPermanentWidget(author);

    ui->actionCopy->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionPaste->setEnabled(false);

    // 默认开启自动换行，并让菜单的勾选状态和实际行为保持一致
    ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    ui->actionWordWrap->setChecked(true);

    ui->actionStatusbar->setChecked(true);
    ui->actionToolbar->setChecked(true);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_A_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionFind_triggered()
{
    SerchDialog dlg(this,ui->textEdit);
    dlg.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionNew_triggered()
{

    if(!userEditConfirmed())
        return;

    filePath = "";
    ui->textEdit->clear();
    this->setWindowTitle(tr("新建文本文件-编辑器"));


    textChanged = false;

}


void MainWindow::on_actionOpen_triggered()
{

    if(!userEditConfirmed())
        return;

    QString filename = QFileDialog::getOpenFileName(this,"打开文件",".",tr("Text files(*.txt);;All(*.*)"));
    if(filename.isEmpty())              // 用户取消了对话框
        return;

    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"..","打开文件失败");
        return;
    }

    filePath = filename;


    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setPlainText(text);    // 用 setPlainText，避免和编辑器里已有内容叠加
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());

    textChanged = false;



}


void MainWindow::on_actionSave_triggered()
{

    if(filePath == ""){
        QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text files(*.txt)"));
        if(filename.isEmpty())          // 用户取消了对话框
            return;

        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text)){

            QMessageBox::warning(this,"..","保存文件失败");
            return;

        }
        file.close();
        filePath = filename;

    }

    QFile file(filePath);


    if(!file.open(QFile::WriteOnly | QFile::Text)){

        QMessageBox::warning(this,"..","保存文件失败");
        return;

    }


    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();


    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());

    textChanged =false;


}


void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text files(*.txt)"));
    if(filename.isEmpty())              // 用户取消了对话框
        return;

    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)){

        QMessageBox::warning(this,"..","保存文件失败");
        return;

    }

    filePath = filename;

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();


    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());

    textChanged = false;                // 另存完成后修改状态复位


}


void MainWindow::on_textEdit_textChanged()
{
    if(!textChanged){
        this->setWindowTitle("*" + this->windowTitle());
        textChanged=true;
    }
}


// 点窗口右上角的叉、或者执行"退出"时都会走这里
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(userEditConfirmed())
        event->accept();
    else
        event->ignore();
}

bool MainWindow::userEditConfirmed()
{
    if(textChanged){

        QMessageBox msg(this);

        QString path =( filePath != "" )? filePath : "无标题.txt";


        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setText(QString("是否将更新保存到\n")+"\"" + path + "\"?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        int r = msg.exec();
        switch (r) {
        case QMessageBox::Yes:
            on_actionSave_triggered();

            break;

        case QMessageBox::No:
            textChanged = false;
            break;

        case QMessageBox::Cancel:
            return false;
        }


    }


    return true;
}




void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
    ui->actionPaste->setEnabled(true);
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);                        // 有选中内容时"剪切"才可用
    ui->actionPaste->setEnabled(ui->textEdit->canPaste());
}


void MainWindow::on_textEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}


void MainWindow::on_textEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}


void MainWindow::on_action_3_triggered()//字体颜色
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit{color: %1}").arg(color.name()));
    }
}


void MainWindow::on_action_5_triggered()//编辑器背景颜色
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit{background-color: %1}").arg(color.name()));
    }

}


void MainWindow::on_action_6_triggered()//字体背景颜色
{

}


void MainWindow::on_actionWordWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();
    if(mode == QPlainTextEdit::NoWrap){
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        ui->actionWordWrap->setChecked(true);

    }else{
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionWordWrap->setChecked(false);
    }



}


void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok,this);

    if(ok)
        ui->textEdit->setFont(font);
}


void MainWindow::on_actionToolbar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionToolbar->setChecked(!visible);

}


void MainWindow::on_actionStatusbar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionStatusbar->setChecked(!visible);
}


void MainWindow::on_actionSelectAll_triggered()
{
    ui->textEdit->selectAll();
}


void MainWindow::on_actionExit_triggered()
{
    close();      // 交给 closeEvent 统一处理"是否需要保存"
}

