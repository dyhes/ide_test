#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "mywidget.h"


#include <QMainWindow>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QTextEdit>
#include <QSplitter>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QTextBrowser>
#include <QPushButton>
#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QTextCodec>
#include <QFileDialog>
#include <string.h>
#include <QTextBrowser>
#include <QProcess>
#include<matching.h>
#include<QDateTime>
 #include<QTimer>
#include <QAction>
#include <QMenu>
#include<myhighlighter.h>
enum MyCode
{
    utf_8, GBK
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void write_data(QString cmd);
    void but1_clicked();

public:
    MyWidget *myWidget;
    QTimer *myTimer;
    QProcess * process;
    QString path;   // 存放地址
    QString pathName; //存放文件名

    int f=1; //判断只新建一个窗口
    QWidget *new_w; //新建一个窗口
    QLineEdit *line1; //新建窗口内的控件
    QFileInfo fileInfo;
    QLabel *label2;//新建文件时显示错误信息
    QTextCodec *codec;\
    enum MyCode flag;
    //删除右键菜单
    QAction *actionDelete;

    //添加右键菜单
    QAction *actionNew;

private slots:
    void read_data();
    void on_actionxianjian_triggered();

    void on_actioniuhh_triggered();

    void on_actionAD_triggered();

    void on_actionAD_2_triggered();

    void on_actionADA_triggered();

    void on_actionhjjj_triggered();

    void on_actionde_triggered();

    void on_actiongj_triggered();

    void on_actiongj_2_triggered();

    void on_actiongy_triggered();

    void on_actionhui_triggered();

    void on_actionjui_triggered();

    void on_actionjjj_triggered();

    void compileSlot ( ) ;
    
	void currentEditorClose ( ) ; 

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;

protected:
      bool eventFilter(QObject *obj, QEvent *ev);
};

#endif // MAINWINDOW_H
