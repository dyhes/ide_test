#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mywidget.h"
#include <QDebug>
#include<matching.h>
#include <ideeditor.h>
#include<QString>
#include<myhighlighter.h>
/*
#include<stdio.h>
int main()
{
printf("123");
return 0;
}
*/




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myWidget = new MyWidget();

    flag = utf_8;
             // 字符编码指针初始化
    codec = QTextCodec::codecForName("GBK");
    process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start("cmd.exe");

    //界面上显示时间
    myTimer=new QTimer(this);
    myTimer->start(1000);
        QLabel * btn = new QLabel(this);
        connect(myTimer , QTimer::timeout, [=](){
            QDateTime Date_Time =QDateTime::currentDateTime();
            QString Time_system = Date_Time.toString("yyyy-MM-dd hh:mm:ss ddd");
            btn->setText(Time_system);
        });
        //statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
        ui->statusBar->addPermanentWidget(btn);

    //颜色修改
    ui->mainToolBar->setStyleSheet("background-color:rgb(34,34,34);");//更改工具栏背景颜
    ui->statusBar->setStyleSheet("background-color:rgb(34,34,34);color:white");//更改状态栏背景颜
    //菜单栏最上面（QMenuBar）要与下面打开的（QMenu）一起更改
    this->setStyleSheet("QMenuBar{"
                                  "background:rgb(176,176,176);"
                                  "color:rgb(0,0,0);"

                                  "}"
                                  "QMenuBar::item{"
                                  "background:rgb(176,176,176);"

                                  "}"
                                  "QMenuBar::item:selected{"
                                  "background:rgb(50,50,50);"
                                  "color:white;"
                                  "}"
                                   "QMenu{"
                                   "background:#d9d9d9;"
                                   "color:rgb(0,0,0);"

                                    "}"

                                   "QMenu::item:selected{" //鼠标移入
                                    "background:#781a1a;"
                                    "color:white;"
                        /*"border:1px solid rgb(165,171,184);"*/
                                    "}"
                                "QMenu::item:disabled{" //不可点击
                                "background:rgb(224,224,224);"
                                "color:rgb(153,153,153);"
                                "}"
                               );

    connect(process,&QProcess::readyRead,this,&MainWindow::read_data);                                    //读命令行数据
    connect(process,&QProcess::readyReadStandardOutput,this,&MainWindow::read_data);                      //读命令行标准数据（兼容）

    setCentralWidget(myWidget->widgetInit());//将myWidget设置为中心控件
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(on_actionxianjian_triggered()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(on_actioniuhh_triggered()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(on_actionAD_triggered()));
    connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(on_actiongj_2_triggered()));
    connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(on_actiongy_triggered()));
    connect(ui->actionShear, SIGNAL(triggered()), this, SLOT(on_actiongj_triggered()));
    connect(ui->actionFind, SIGNAL(triggered()), this, SLOT(on_actionhui_triggered()));
    connect(ui->actionReplace, SIGNAL(triggered()), this, SLOT(on_actionjui_triggered()));
    connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(on_actionhjjj_triggered()));
    connect(ui->actionRenew, SIGNAL(triggered()), this, SLOT(on_actionde_triggered()));
    connect(ui->actionRun, SIGNAL(triggered()), this, SLOT(on_actionjjj_triggered()));
    connect(ui->actionss , &QAction::triggered , this , &MainWindow::compileSlot) ;

    this->installEventFilter(this);

    /*初始化右键菜单删除按钮*/
    actionDelete = new QAction("删除", this);
    /*初始化右键菜单新建按钮*/
    actionNew = new QAction("新建", this);
    // 删除选中行
       connect(actionDelete,&QAction::triggered,this,[=](){
           //QMessageBox::StandardButton result=QMessageBox::question(&widget, "Title","text");
           QMessageBox message(QMessageBox::Question, tr("QMessageBox:question"), "Are you sure you want to delete the file?", QMessageBox::Yes | QMessageBox::No);
           if(message.exec() == QMessageBox::Yes)
           {
               int row = myWidget->listWidget->currentRow();
               QListWidgetItem *aItem =myWidget->listWidget->takeItem(row);
               delete aItem;
               QWidget* widget = myWidget->stackedWidget->widget(row);
               myWidget->stackedWidget->removeWidget(widget);
               widget->deleteLater();

           }});
       // 新建行
          connect(actionNew,&QAction::triggered,this,&MainWindow::on_actionxianjian_triggered);
       connect(myWidget->listWidget, &QWidget::customContextMenuRequested, this, &MainWindow::on_listWidget_customContextMenuRequested);
       myWidget->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);


}

MainWindow::~MainWindow()
{
    delete ui;
}
//新建文本
void MainWindow::on_actionxianjian_triggered()
{

    new_w = new QWidget();
    new_w->setWindowTitle("新建文件");
    new_w->resize(400,300);
    Qt::WindowFlags flags = this->windowFlags();
    new_w->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    QVBoxLayout *vBox = new QVBoxLayout(new_w);
    QHBoxLayout *hBox = new QHBoxLayout(new_w);

    QLabel *label1 = new QLabel(new_w);
    label1->setText("文件名：");
    line1 = new QLineEdit(new_w);

    hBox->addWidget(label1);
    hBox->addWidget(line1);
    label2 = new QLabel(new_w);
    label2->setFixedHeight(20);//固定高度
    QPushButton *but1 = new QPushButton(new_w);
    but1->setText("新建文件");
    connect(but1,&QPushButton::clicked,[=](){emit but1_clicked();});
    but1->setStyleSheet("QPushButton:hover{"
                               "background-color:rgb(131,26,26);"
                               "border: 1px solid rgb(11 , 137 , 234);"
                               "color:rgb(255,255,255);"
                               "}"
                               "QPushButton:pressed{"
                                   "color:white;"
                                   "background-color:rgb(14 , 135 , 228);"
                                   "border: 1px solid rgb(12 , 138 , 235);"
                               "}");
    vBox->addLayout(hBox);
    vBox->addWidget(label2);
    vBox->addWidget(but1);
    new_w->show();

}

//打开文件
void MainWindow::on_actioniuhh_triggered()
{
    // 打开文件，获取文件路径
    path = QFileDialog::getOpenFileName ( nullptr , "select open file" , "/" , "c files(*.c);;cpp files(*.cpp);;txt files(*.txt)" , nullptr , 0 )  ;


    qDebug() << "path=" << path;
    if(path.isEmpty())
    {
        // 没有选择路径直接退出
        return;
    }

    // 字符编码转换
    char *fileName = codec->fromUnicode(path).data();
    // 打开文件并读取内容,并放进编辑区
    FILE *fp = fopen(fileName, "rb");
    if(fp == NULL)
    {
        qDebug() << "open error";
        return;
    }

    char buf[1024*5];
    QString str = "";

    // 读取文件
    while (1) {
        memset(buf, 0, sizeof (buf));
        fgets(buf, sizeof (buf), fp);

        // 根据标志位改变字符编码方式
        if(flag == utf_8)
        {
            str += buf;
        }
        else if(flag == GBK)
        {
            str += codec->toUnicode(buf);
        }
        if(feof(fp))
        {
            break;
        }
    }
    fileInfo = QFileInfo(path);
        //文件名
    pathName = fileInfo.fileName();
    new QListWidgetItem(pathName, myWidget->listWidget);

    IDEEditor *editor = new IDEEditor ( ) ;
    editor->textedit->setPlainText ( str ) ;
    myWidget->stackedWidget->addWidget(editor);

    // 关闭文件
    fclose(fp);
    fp = NULL;
}

//保存
void MainWindow::on_actionAD_triggered()
{
    if ( myWidget->stackedWidget->count() == 0 ) return ;
    if(path.isEmpty())
    {
        path = QFileDialog::getSaveFileName(nullptr , "select save file" , "/" , "cpp files(*.cpp)" , nullptr , 0);
    }
    const char *fileName = codec->fromUnicode(path).data();

    FILE *fp = fopen(fileName, "wb");
    if(fp == NULL)
    {
        qDebug() << "on_actionSAVE_triggered open file err";
    }

    IDEEditor *editor = ( IDEEditor* ) ( myWidget->stackedWidget->currentWidget() ) ;
    QString str = editor->textedit->toPlainText();
    const char *buf = str.toStdString().data();
    fputs(buf, fp);
    fclose(fp);
}
//另存为
void MainWindow::on_actionAD_2_triggered()
{
    if ( myWidget->stackedWidget->count() == 0 ) return ;

    path = QFileDialog::getSaveFileName(nullptr , "select save file" , "/" , "cpp files(*.cpp)" , nullptr , 0);
    if(path.isEmpty())
    {
        return;
    }
    const char *fileName = codec->fromUnicode(path).data();
    FILE *fp = fopen(fileName, "wb");
    if(fp == NULL)
    {
        qDebug() << "on_actionSAVE_AS_triggered open file err";
    }

    IDEEditor *editor = ( IDEEditor* ) ( myWidget->stackedWidget->currentWidget() ) ;
    QString str = editor->textedit->toPlainText();
    const char *buf = str.toStdString().data();
    fputs(buf, fp);
    // 关闭文件
    fclose(fp);
}
//关闭所有
void MainWindow::on_actionADA_triggered()
{
    int counter =myWidget->listWidget->count();
        for(int index=0;index<counter;index++)
        {
            QListWidgetItem *item = myWidget->listWidget->takeItem(0);
                delete item;

        }
        for(int i = myWidget->stackedWidget->count(); i >= 0; i--)

           {

               QWidget* widget = myWidget->stackedWidget->widget(i);

               myWidget->stackedWidget->removeWidget(widget);

               widget->deleteLater();

           }

}
//撤销
void MainWindow::on_actionhjjj_triggered()
{
    if ( myWidget->stackedWidget->count() == 0 ) return ;
    IDEEditor *editor = ( IDEEditor* ) ( myWidget->stackedWidget->currentWidget() ) ;
    editor->textedit->undo();
}
//恢复
void MainWindow::on_actionde_triggered()
{
    if ( myWidget->stackedWidget->count() == 0 ) return ;
    IDEEditor *editor = ( IDEEditor* ) ( myWidget->stackedWidget->currentWidget() ) ;
    editor->textedit->redo();
}
//剪切
void MainWindow::on_actiongj_triggered()
{
    if ( myWidget->stackedWidget->count() == 0 ) return ;
    IDEEditor *editor = ( IDEEditor* ) ( myWidget->stackedWidget->currentWidget() ) ;
    editor->textedit->cut();
}
//复制
void MainWindow::on_actiongj_2_triggered()
{
    if ( myWidget->stackedWidget->count() == 0 ) return ;
    IDEEditor *editor = ( IDEEditor* ) ( myWidget->stackedWidget->currentWidget() ) ;
    editor->textedit->copy();
}
//粘贴
void MainWindow::on_actiongy_triggered()
{
    if ( myWidget->stackedWidget->count() == 0 ) return ;
    IDEEditor *editor = ( IDEEditor* ) ( myWidget->stackedWidget->currentWidget() ) ;
    editor->textedit->paste();
}

//显示查找
void MainWindow::on_actionhui_triggered()
{
    myWidget->tabwidget->setCurrentIndex(1);
}
//显示替换
void MainWindow::on_actionjui_triggered()
{
    myWidget->tabwidget->setCurrentIndex(1);
}

//编译
void MainWindow::compileSlot()
{
    if ( myWidget->stackedWidget->count() == 0 ) return ;
    on_actionAD_triggered() ;
    QString exe = path;
    if (exe.endsWith(".cpp") )
        exe.replace(".cpp", ".exe");
    else if ( exe.endsWith(".c") )
        exe.replace(".c", ".exe");
    else if ( exe.endsWith(".txt") )
        exe.replace(".txt",".exe" ) ;

    exe.replace("/","\\");
    QString del=QString("%1 %2").arg("del").arg(exe);
    write_data(del);
//    qDebug()<<del;
    if(path.isEmpty())
    {
        QMessageBox::critical(this, tr("warning"), tr("请先保存当前文件"));
        //this->save();
    }
    else{
        myWidget->b1->setText("building...");

        QString cmd = QString("gcc %1 -o %2").arg(path).arg(exe);
        write_data(cmd);//编译
        }
}

//运行
void MainWindow::on_actionjjj_triggered()
{
    if ( myWidget->stackedWidget->count() == 0 ) return ;
    on_actionAD_triggered() ;
    QString exe = path;
    if (exe.endsWith(".cpp") )
        exe.replace(".cpp", ".exe");
    else if ( exe.endsWith(".c") )
        exe.replace(".c", ".exe");
    else if ( exe.endsWith(".txt") )
        exe.replace(".txt",".exe" ) ;


    exe.replace("/","\\");
    QString del=QString("%1 %2").arg("del").arg(exe);
    write_data(del);

       if(path.isEmpty())
       {
           QMessageBox::critical(this, tr("warning"), tr("请先保存当前文件"));
           //this->save();
       }
       else{
           myWidget->b1->setText("building...");

           QString cmd = QString("gcc %1 -o %2").arg(path).arg(exe);
           write_data(cmd);//编译
           write_data(exe);//运行
           }
}



void MainWindow::currentEditorClose ( )
{
    if ( myWidget->stackedWidget->count() == 0 ) return ;
    myWidget->stackedWidget->removeWidget ( myWidget->stackedWidget->currentWidget() ) ;
    myWidget->listWidget->removeItemWidget ( myWidget->listWidget->currentItem() ) ;
}

//从控制台读取数据
void MainWindow::read_data()
{
    /* 接收数据 */
    QByteArray bytes = process->readAll();
    /* 显示 */
    QString msg = QString::fromLocal8Bit(bytes);
    myWidget->b1->append(msg);
    myWidget->b1->update();
    /* 信息输出 */
    qDebug() << "Success:read_data:" << msg << endl;
}
//向控制台写入数据
void MainWindow::write_data(QString cmd)
{
    QString command = cmd+"\r\n";
    QByteArray qbarr = command.toLatin1();
    char *ch = qbarr.data();
    qint64 len = command.length();
    process->write(ch, len);
}


//点击新建文件按钮之后
void MainWindow::but1_clicked()
{
    QString ss;
    ss=line1->text();
    if(ss.isEmpty()){
        //QMessageBox::critical(this, tr("warning"), tr(" 请先输入文件名"));
        label2->setStyleSheet("background-color:rgb(254,254,0);font-size:15px;color:black");
        label2->setText("请先输入文件名");
    }
    else{
        //新建一个标签
        new QListWidgetItem(ss, myWidget->listWidget);

        //新建一个提供编辑的页面
        //qDebug ( ) << "ready to create new ideeditor" ;
        IDEEditor *editor = new IDEEditor ( myWidget ) ;
        myWidget->stackedWidget->addWidget(editor);
        new_w->close();
        f=1 ;
    }



}
//主窗口关闭时，关闭结束程序，关闭所有窗口
bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    /*判断发生事件的是否为主窗口*/
    if(obj==this) {
        /*判断事件的类型是否为关闭事件*/
        if (ev->type() == QEvent::Close){
            qApp->quit();//主窗口关闭事件,程序退出,其他窗口将被关闭
            return true; //返回true代表事件已被处理不需要向下传递，返回false代表事件未被处理需要向下传递由别的组件处理
        }
    }
    /*不是我们关心的事件，交由QMainWindow的事件过滤器处理*/
    return QMainWindow::eventFilter(obj, ev);
}

//列表的右键菜单
void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
       QMenu *menuList = new QMenu(this);
       //添加 Actions 创建菜单项
       //QAction *NewAction = new QAction("NewFile", this);
           menuList->addAction(actionNew);

       //menuList->addAction(NewAction);

       menuList->addSeparator();
           QList<QListWidgetItem*> items = myWidget->listWidget->selectedItems();
           qDebug()<<items.count();

       if(items.count()>0) //如果有item则添加"修改"菜单 [1]*
           {

               menuList->addAction(actionDelete);
           }


       //在鼠标光标位置显示右键快捷菜单
   //    menuList->exec(pos);
       menuList->exec(QCursor::pos());
       //将不用对象从内存删除
       delete menuList;

}



