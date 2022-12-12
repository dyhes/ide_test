#include "mywidget.h"
#include <QScrollBar>
#include <QDebug>
#include <QMessageBox>
#include<myeditor.h>
#include <QFontMetrics>
#include <ideeditor.h>
//滚动条样式表宏定义
#define myScrollBarStyle\
"QScrollBar:vertical\
{\
       border: none;\
       background: black;\
       width: 6px;\
}\
QScrollBar::handle:vertical\
{\
       background-color: rgba(255,255,255,1);\
       min-height: 30px;\
      border-radius:3px;\
      width: 6px;\
}\
QScrollBar::add-line:vertical\
{\
       border: none;\
       height: 0px;\
}\
 QScrollBar::sub-line:vertical\
{\
       border: none;\
       height: 0px;\
}\
QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical\
{\
    background:transparent;\
      border-radius:3px;\
}"\
"QScrollBar:horizontal{background-color:transparent;width:140px;height:12px;padding-left:12px;padding-right:12px;}"   //整个水平滚动条背景样式\
"QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background-color:transparent;}"         //滚动条左面和右面区域样式\
"QScrollBar::handle:horizontal{background:rgba(75,120,154,0.8);border:1px solid rgba(82,130,164,1);}"    //滑块区域样式\
"QScrollBar::handle:horizontal:hover,QScrollBar::handle:horizontal:pressed{background:rgba(75,120,154,1);border:1px solid rgb(255,255,255);}"//鼠标滑过滑块样式\
"QScrollBar::sub-line:horizontal{subcontrol-position:left;height:12px;width:12px;background:rgba(75,120,154,0.8) url(:/1.png);}"//向左箭头样式\
"QScrollBar::sub-line:horizontal:hover,QScrollBar::sub-line:horizontal:pressed{height:20px;width:20px;background:rgba(75,120,154,1) url(:/1.png);border:1px solid rgb(255,255,255);}"//向左箭头样式\
"QScrollBar::add-line:horizontal{height:12px;width:20px;background:rgba(75,120,154,0.8) url(:/1.png);}"//向右箭头样式\
"QScrollBar::add-line:horizontal:hover,QScrollBar::add-line:horizontal:pressed{height:20px;width:20px;background:rgba(75,120,154,1) url(:/1.png);border:1px solid rgb(255,255,255);}"//向右箭头样式


MyWidget::MyWidget()
{
    //listWidget = new QListWidget();


}
QWidget * MyWidget::widgetInit()
{
    /* 水平布局实例化 */
    hBoxLayout = new QHBoxLayout();

    /* 垂直布局实例化 */
    vBoxLayout = new QVBoxLayout();
    vBoxLayout1 = new QVBoxLayout();

    /* 堆栈部件实例化 */
    stackedWidget = new QStackedWidget();

    //项目标签实例化
    QLabel *label1 = new QLabel();
    label1->setText("项目 ");

    /* 列表实例化 *///在这里显示文件名
    listWidget = new QListWidget();
    int j=0;



    //列表与标签的垂直布局
    vBoxLayout1->addWidget(label1);
    vBoxLayout1->addWidget(listWidget);

    tabwidget = new QTabWidget();
       //QFrame *page1 = new QFrame(tabwidget);
       //加第一个
       QWidget *page1 = new QWidget();
       QHBoxLayout *h1 = new QHBoxLayout(page1);
       b1 = new QTextBrowser(page1);
       h1->addWidget(b1);
       //加第二个
       QWidget *page2 = new QWidget();
       QVBoxLayout *h4 = new QVBoxLayout(page2);//申请布局器的位置也会影响代码的书写
       QHBoxLayout *h2 = new QHBoxLayout(page2);
       QLabel *labelName2 = new QLabel(page2);
       labelName2->setText("查找内容: ");
       h2->addWidget(labelName2);
       searchEditer = new QLineEdit(page2);
       h2->addWidget(searchEditer);
       QPushButton *pushButton1 = new QPushButton(page2);
       pushButton1->setText("查找下一个");
       pushButton1->setStyleSheet("QPushButton:hover{"
                                  "background-color:rgb(131,26,26);"
                                  "border: 1px solid rgb(11 , 137 , 234);"
                                  "color:rgb(255,255,255);"
                                  "}"
                                  "QPushButton:pressed{"
                                      "color:white;"
                                      "background-color:rgb(14 , 135 , 228);"
                                      "border: 1px solid rgb(12 , 138 , 235);"
                                  "}");
       QPushButton *pushButton2 = new QPushButton(page2);
       pushButton2->setText("查找上一个");
       pushButton2->setStyleSheet("QPushButton:hover{"
                                  "background-color:rgb(131,26,26);"
                                  "border: 1px solid rgb(11 , 137 , 234);"
                                  "color:rgb(255,255,255);"
                                  "}"
                                  "QPushButton:pressed{"
                                      "color:white;"
                                      "background-color:rgb(14 , 135 , 228);"
                                      "border: 1px solid rgb(12 , 138 , 235);"
                                  "}");
       h2->addWidget(pushButton1);
       h2->addWidget(pushButton2);
       h4->addLayout(h2);
       QHBoxLayout *h3 = new QHBoxLayout(page2);
       QLabel *labelName3 = new QLabel(page2);
       labelName3->setText("替换: ");
       h3->addWidget(labelName3);
       replaceEditer = new QLineEdit(page2);
       h3->addWidget(replaceEditer);
       QPushButton *pushButton3 = new QPushButton(page2);
       pushButton3->setText("替换");
       pushButton3->setStyleSheet("QPushButton:hover{"
                                  "background-color:rgb(131,26,26);"
                                  "border: 1px solid rgb(11 , 137 , 234);"
                                  "color:rgb(255,255,255);"
                                  "}"
                                  "QPushButton:pressed{"
                                      "color:white;"
                                      "background-color:rgb(14 , 135 , 228);"
                                      "border: 1px solid rgb(12 , 138 , 235);"
                                  "}");
       QPushButton *pushButton4 = new QPushButton(page2);
       pushButton4->setText("替换全部");
       pushButton4->setStyleSheet("QPushButton:hover{"
                                  "background-color:rgb(131,26,26);"
                                  "border: 1px solid rgb(11 , 137 , 234);"
                                  "color:rgb(255,255,255);"
                                  "}"
                                  "QPushButton:pressed{"
                                      "color:white;"
                                      "background-color:rgb(14 , 135 , 228);"
                                      "border: 1px solid rgb(12 , 138 , 235);"
                                  "}");
       h3->addWidget(pushButton3);
       h3->addWidget(pushButton4);
       h4->addLayout(h3);

       //lineEdit控件与函数相连接
       connect ( pushButton1 , &QPushButton::clicked , this , &MyWidget::nextOne ) ;
       connect ( pushButton2 , &QPushButton::clicked , this , &MyWidget::previousOne ) ;
       connect ( pushButton3 , &QPushButton::clicked , this , &MyWidget::replaceOne ) ;
       connect ( pushButton4 , &QPushButton::clicked , this , &MyWidget::replaceAll ) ;


     tabwidget->addTab(page1,"编译输出");
     tabwidget->addTab(page2,"查找");



     //添加到垂直布局
     //vBoxLayout->addLayout(hb);
     vBoxLayout->addWidget(stackedWidget);
     vBoxLayout->addWidget(tabwidget);
    splitterMain = new QSplitter(Qt::Horizontal,nullptr); // 水平布置
    /* 设置列表的最大宽度 */
    listWidget->setMaximumWidth(200);
    /* 添加到水平布局 */
    hBoxLayout->addLayout(vBoxLayout1);
    hBoxLayout->addLayout(vBoxLayout);

    /* 利用listWidget的信号函数currentRowChanged()与
     * 槽函数setCurrentIndex(),进行信号与槽连接
     */
    connect(listWidget, SIGNAL(currentRowChanged(int)),stackedWidget, SLOT(setCurrentIndex(int)));

    QWidget *widget = new QWidget();
    widget->setLayout(hBoxLayout);
    return widget;

}


//-------------------以下为搜索
void MyWidget::searchForWord ( ) //搜索指定字符串
{
    IDEEditor *editor = ( IDEEditor* ) ( stackedWidget->currentWidget() ) ;

    QString searchWord = searchEditer->text ( ) ; 
    if ( tabwidget->currentIndex() != 1 || searchWord.length() == 0 ) return ; 
    QString allWord = editor->textedit->toPlainText ( ) ;

    QTextCursor cur = editor->textedit->textCursor ( ) ;
    int index = -1 ;
//    qDebug ( ) << "search in " << index ;
    if ( direction == 1 )
    {

        index = allWord.indexOf(searchWord,cur.position(),Qt::CaseSensitive) ;
        if ( index >= 0 )
        {
            cur.setPosition(index);
            cur.setPosition(index+searchWord.length(),QTextCursor::KeepAnchor);
            editor->textedit->setTextCursor(cur);
        }
    }
    else
    {
        index = allWord.lastIndexOf(searchWord,cur.position() - searchWord.length(), Qt::CaseSensitive) ;
        qDebug ( ) << cur.position() ;
        if ( index >= 0 )
        {
            cur.setPosition(index+searchWord.length());
            cur.setPosition(index, QTextCursor::KeepAnchor);
            editor->textedit->setTextCursor(cur);
        }
    }

    if ( index < 0 )
    {
        QMessageBox hint(this) ;
        hint.setWindowTitle("Find");
        hint.setText("Can not find any more...");
        hint.setStandardButtons(QMessageBox::Ok);
        hint.exec();
    }

}

void MyWidget::resetIndex ( ) 
{

}

void MyWidget::previousOne ( ) //前一个
{
    direction = -1 ;
    searchForWord ( ) ;
}

void MyWidget::nextOne ( ) //后一个
{
    direction = 1 ;
    searchForWord();
}

void MyWidget::replaceOne ( ) //替换当前
{
    IDEEditor *editor = ( IDEEditor* ) ( stackedWidget->currentWidget() ) ;

    QString replaceWord = replaceEditer->text() ;
    QTextCursor cur = editor->textedit->textCursor() ;

    if ( replaceWord.length() == 0 ) return ;

    if ( cur.selectedText() == searchEditer->text() )
    {
        editor->textedit->insertPlainText(replaceWord);
    }
    else
    {
        nextOne ( ) ;
    }
}

void MyWidget::replaceAll ( ) //替换全部
{
    IDEEditor *editor = ( IDEEditor* ) ( stackedWidget->currentWidget() ) ;

    QString replaceWord = replaceEditer->text() ;
    QTextCursor cur = editor->textedit->textCursor() ;
    if ( replaceWord.length() == 0 ) return ;

    cur.setPosition(0);
    editor->textedit->setTextCursor(cur);

    while ( 1 )
    {
        nextOne();
        if ( editor->textedit->textCursor().selectedText() != searchEditer->text() ) break ;
        editor->textedit->insertPlainText(replaceWord);
    }
}
//----------------------------以上为搜索
