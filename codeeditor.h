#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>

// 带行号栏的编辑器：参考 Qt 官方 Code Editor 示例，
// 在左侧留出一条区域，把每个文本块（行）的序号画上去。
class CodeEditor;

class LineNumberArea : public QWidget
{
public:
    explicit LineNumberArea(CodeEditor *editor);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *codeEditor;
};

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);

    // 行号栏需要多宽（按最大行号的位数算）
    int lineNumberAreaWidth();

    // 由 LineNumberArea 调用，负责把行号画出来
    void lineNumberAreaPaintEvent(QPaintEvent *event);

    // 显示/隐藏行号栏（对应菜单里的“显示行号”）
    void setLineNumberAreaVisible(bool visible);
    bool isLineNumberAreaVisible() const { return lineNumberVisible; }

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
    bool lineNumberVisible = true;
};

#endif // CODEEDITOR_H
