#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QCloseEvent;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    // 关闭窗口时统一走“是否保存修改”的确认流程
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_action_A_triggered();

    void on_actionFind_triggered();

    void on_actionReplace_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_textEdit_textChanged();

    void on_textEdit_cursorPositionChanged();

    void on_actionCopy_triggered();

    void on_actionUndo_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_actionRedo_triggered();

    void on_textEdit_copyAvailable(bool b);

    void on_textEdit_redoAvailable(bool b);

    void on_textEdit_undoAvailable(bool b);

    void on_action_3_triggered();

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_actionWordWrap_triggered();

    void on_actionFont_triggered();

    void on_actionToolbar_triggered();

    void on_actionStatusbar_triggered();

    void on_actionSelectAll_triggered();

    void on_actionExit_triggered();

    void on_actionAutoSave_triggered();

    void on_action_L_triggered();      // 显示/隐藏行号

private:
    Ui::MainWindow *ui;

    QLabel statusCursorLabel;
    QLabel statusLabel;

    QString filePath;

    bool textChanged;

    // 定时自动保存用
    QTimer *autoSaveTimer = nullptr;

    bool userEditConfirmed();

    // 刷新状态栏里的字数/行数与行列号
    void updateStatusInfo();

    // 定时器到点时调用：有未保存修改且已有文件路径才真正写盘
    void autoSave();


};
#endif // MAINWINDOW_H
