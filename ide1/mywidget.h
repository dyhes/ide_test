#ifndef MYWIDGET_H
#define MYWIDGET_H
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

class MyWidget : public QWidget
{
public:
    MyWidget();
    QWidget * widgetInit();


public:
    /* 水平布局 */
    QHBoxLayout *hBoxLayout;
    /*垂直布局器*/
    QVBoxLayout *vBoxLayout;
    QVBoxLayout *vBoxLayout1;
    /* 列表视图 */
    QListWidget *listWidget;
    /* 堆栈窗口部件 */
    QStackedWidget *stackedWidget;

    /*界面分割器*/
    QSplitter *splitterMain;
    /*右下方的分页显示*/
    QTabWidget *tabwidget;

    QTextBrowser *b1;//编译输出


    int direction ; //搜索的方向
    QLineEdit *searchEditer ; //搜索替换的两个文本框
    QLineEdit *replaceEditer ;
public slots :

    //搜索替换有关槽函数
    void searchForWord ( ) ;
    void resetIndex ( ) ;
    void previousOne ( ) ;
    void nextOne ( ) ;
    void replaceOne ( ) ;
    void replaceAll ( ) ;
//--------------------------------

};

#endif // MYWIDGET_H
