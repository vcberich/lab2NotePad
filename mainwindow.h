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

private:
    Ui::MainWindow *ui;

    QLabel statusCursorLabel;
    QLabel statusLabel;

    QString filePath;

    bool textChanged;


    bool userEditConfirmed();

    // 刷新状态栏里的字数/行数与行列号
    void updateStatusInfo();


};
#endif // MAINWINDOW_H
