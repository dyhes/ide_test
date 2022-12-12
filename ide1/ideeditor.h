#ifndef IDEEDITOR_H
#define IDEEDITOR_H
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QTextEdit>
#include <QSplitter>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include<QComboBox>
#include<QTextBrowser>
#include <QPushButton>
#include <QWidget>
#include<myhighlighter.h>
#include <QList>
#include<QPlainTextEdit>
#include<myeditor.h>
#include <matching.h>
#include <QTextCharFormat>
#include <linenumberwidget.h>

class IDEEditor : public QWidget
{
    Q_OBJECT

public:
    /*文本框*/
    myEditor *textedit ;
    /*行数显示框*/
    LineNumberWidget *lineNumberWidget ;
    int lastLine;//上一次的行

    Matching *m ;//括号匹配集
    QTextCharFormat oldFormat ;

public:
    explicit IDEEditor(QWidget *parent = nullptr);
    void setFormat(int pos,int result);
private:
    int length;
    int prelen;


signals:

public slots:
    void onTextChange();//行数显示槽函数
    void slot_textChange();//行数框宽度适应
    void autoMatch();
    //------------括号匹配
    void onCursorSelect();
    void transToMatching();
    //------------括号匹配
    void textchangedSlot();
    void cursorchangedSlot();

//    void lineNumberChanged ( ) ;
    void fold();//折叠
};

#endif // IDEEDITOR_H
