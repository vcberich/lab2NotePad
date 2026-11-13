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
#include <QTextCharFormat>
#include <QTextCursor>
#include <QPalette>
#include <QTimer>

namespace {
// 定时自动保存的间隔（毫秒）
constexpr int AutoSaveIntervalMs = 60 * 1000;
}

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

    // 定时自动保存：默认开启，每 60 秒检查一次；
    // 只有"有未保存的修改"且"已经有关联的文件路径"时才会真正写盘
    autoSaveTimer = new QTimer(this);
    autoSaveTimer->setInterval(AutoSaveIntervalMs);
    connect(autoSaveTimer, &QTimer::timeout, this, &MainWindow::autoSave);
    autoSaveTimer->start();
    ui->actionAutoSave->setChecked(true);

    // “显示行号”做成可勾选，默认显示
    ui->action_L->setCheckable(true);
    ui->action_L->setChecked(true);

    updateStatusInfo();
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
    ReplaceDialog dlg(this, ui->textEdit);
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

    updateStatusInfo();
}


void MainWindow::on_textEdit_cursorPositionChanged()
{
    updateStatusInfo();
}


void MainWindow::updateStatusInfo()
{
    const QString text = ui->textEdit->toPlainText();
    statusLabel.setText(QString("length: %1    lines: %2")
                            .arg(text.length())
                            .arg(ui->textEdit->blockCount()));

    const QTextCursor cursor = ui->textEdit->textCursor();
    statusCursorLabel.setText(QString("Ln: %1    Col: %2")
                                  .arg(cursor.blockNumber() + 1)
                                  .arg(cursor.positionInBlock() + 1));
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


void MainWindow::on_action_5_triggered()//字体背景色（选中文字的底色）
{
    QColor color = QColorDialog::getColor(Qt::yellow,this,"选择字体背景色");
    if(!color.isValid())
        return;

    QTextCharFormat fmt;
    fmt.setBackground(color);

    QTextCursor cursor = ui->textEdit->textCursor();
    if(cursor.hasSelection())
        cursor.mergeCharFormat(fmt);            // 只改选中的文字
    else
        ui->textEdit->setCurrentCharFormat(fmt); // 没选中就作用于接下来输入的文字
}


void MainWindow::on_action_6_triggered()//编辑器背景色
{
    QColor color = QColorDialog::getColor(ui->textEdit->palette().color(QPalette::Base),
                                          this,"选择编辑器背景色");
    if(color.isValid()){
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit{background-color: %1}").arg(color.name()));
    }
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


void MainWindow::on_action_L_triggered()
{
    // 勾上显示行号，取消勾选则隐藏
    ui->textEdit->setLineNumberAreaVisible(ui->action_L->isChecked());
}


void MainWindow::on_actionAutoSave_triggered()
{
    if(ui->actionAutoSave->isChecked()){
        autoSaveTimer->start();
        ui->statusbar->showMessage(QString("自动保存已开启（每 %1 秒）").arg(AutoSaveIntervalMs / 1000), 3000);
    }else{
        autoSaveTimer->stop();
        ui->statusbar->showMessage("自动保存已关闭", 3000);
    }
}


void MainWindow::autoSave()
{
    // 没有改动、或者还没另存过（没有路径）就不写盘
    if(!textChanged || filePath.isEmpty())
        return;

    QFile file(filePath);
    if(!file.open(QFile::WriteOnly | QFile::Text))
        return;

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.flush();
    file.close();

    // 存盘后清除"未保存"标记，窗口标题去掉 *
    textChanged = false;
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());

    ui->statusbar->showMessage(QString("已自动保存：%1").arg(QFileInfo(filePath).fileName()), 3000);
}

