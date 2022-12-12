#include "ideeditor.h"
#include "mywidget.h"
#include <QScrollBar>
#include <QDebug>
#include <QMessageBox>
#include<myeditor.h>
#include <matching.h>
#include <QFontMetrics>
#include <QTextCharFormat>
#include <linenumberwidget.h>
#include <QAbstractScrollArea>
#include<QTextBlock>

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



IDEEditor::IDEEditor(QWidget *parent) : QWidget(parent)
{

    textedit = new myEditor ( this ) ;

    /*以下为添加行数框部分*/
    lineNumberWidget = new LineNumberWidget ( ) ;//显示行数的框
    lineNumberWidget->insertPlainText(QStringLiteral("1\n"));
    lineNumberWidget->setFocusPolicy(Qt::NoFocus);
    lineNumberWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lineNumberWidget->verticalScrollBar()->setDisabled(true);
    lineNumberWidget->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );


//    qDebug ( ) << "ready to add linenum widget" ;
    connect(textedit->verticalScrollBar(),&QScrollBar::valueChanged,lineNumberWidget,&LineNumberWidget::lineNumberChange);//将两个文本框的滚动条同步
//    qDebug ( ) << "ready to add layout" ;

    connect(textedit,&QPlainTextEdit::textChanged,this,&IDEEditor::onTextChange);//连接信号与槽
    lastLine = 1;

    //增添滚动条设置
    textedit->setStyleSheet(myScrollBarStyle);
    textedit->moveCursor(QTextCursor::End);
    textedit->setWordWrapMode(QTextOption::NoWrap);


    QFont font;//设置字体大小
    font.setPixelSize(24);
    lineNumberWidget->setFont(font);
    textedit->setFont(font);
    lineNumberWidget->setFixedWidth(lineNumberWidget->font().pixelSize() + 5);//行数框大小初始值
    connect(textedit,&QPlainTextEdit::textChanged,this,&IDEEditor::slot_textChange);//文本框大小自适应

    //设置layout

    QHBoxLayout * hb = new QHBoxLayout();
    hb->setMargin(0);
    hb->setSpacing(0);
    hb->addWidget(lineNumberWidget);
    hb->addWidget(textedit);
    setLayout(hb);
    /*以上为添加行数框部分*/

    //设置选中效果
    QPalette p = textedit->palette ( ) ;
    p.setColor(QPalette::Inactive, QPalette::Highlight, p.color(QPalette::Active, QPalette::Highlight));
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, p.color(QPalette::Active, QPalette::HighlightedText));
    textedit->setPalette(p);

    //---括号匹配部分
    QString txt = textedit->document()->toPlainText();
    m = new Matching();
    m->SetString(txt);
    oldFormat = textedit->currentCharFormat();
    connect(textedit,&myEditor::textChanged,this,&IDEEditor::textchangedSlot);
    connect(textedit,&QPlainTextEdit::cursorPositionChanged,this,&IDEEditor::onCursorSelect);//光标改变时，进行括号匹配
    connect(lineNumberWidget,&LineNumberWidget::cursorPositionChanged,this,&IDEEditor::fold);//函数折叠
}

//行数框宽度自适应槽函数
void IDEEditor::slot_textChange()
{
    int lrow = textedit->document()->lineCount();
    int kuan = 1;
    if(lrow/10 == 0){//一位宽
        kuan = 1;
        lineNumberWidget->setFixedWidth(lineNumberWidget->font().pixelSize() * kuan + 5);
    }
    else if(lrow/10 < 10){//二位宽
        kuan = 2;
        lineNumberWidget->setFixedWidth(lineNumberWidget->font().pixelSize() * kuan - 8);
    }
    else if(lrow/10 < 100){//三位宽
        kuan = 3;
        lineNumberWidget->setFixedWidth(lineNumberWidget->font().pixelSize() * kuan- 19);
    }
    else if(lrow/10 < 10000){//四位宽  2000行保障
        kuan = 4;
        lineNumberWidget->setFixedWidth(lineNumberWidget->font().pixelSize() * kuan - 30);
    }
    else{//其他
        kuan = 5;
        lineNumberWidget->setFixedWidth(lineNumberWidget->font().pixelSize() * kuan - 43);
    }
}

//文本行数变化，槽函数，修改行号
void IDEEditor::onTextChange()
{
    int jsonTextEditRow = textedit->document()->lineCount();
    if(jsonTextEditRow == lastLine)
        return;

    lineNumberWidget->blockSignals(true);
    textedit->blockSignals(true);

    lineNumberWidget->clear();
    QString str;
    ++jsonTextEditRow;
    for (int i = 1; i < jsonTextEditRow; ++i)
    {
        str.append(QString("%1\n").arg(i));
    }
    lineNumberWidget->setPlainText(str);
    //

    lastLine = textedit->document()->lineCount();

    qDebug ( ) << textedit->verticalScrollBar()->value() ;
    lineNumberWidget->verticalScrollBar()->setValue(textedit->verticalScrollBar()->value());
    lineNumberWidget->blockSignals(false);
    textedit->blockSignals(false);
}

//-----------------------------以下为括号匹配
void IDEEditor::onCursorSelect(){
    //qDebug()<<"onCursorSelect";
    QTextCursor txtCursor = textedit->textCursor();

    if ( txtCursor.anchor() != txtCursor.position() ) return ;

    QString selectText = txtCursor.selectedText();
    int pos = txtCursor.position();
    //    qDebug()<<pos; 看下位置对不对
    int result = m->finding(pos);
//    qDebug()<<result;
    if(result>=0){//如果匹配到了
//        qDebug()<<"匹配到了";
        setFormat(pos,result);
    }
    else{
//        qDebug()<<"恢复";
        textedit->blockSignals(true);
        QTextCursor cursor = textedit->textCursor();
        cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::End,QTextCursor::KeepAnchor);
        textedit->setTextCursor(cursor);
        cursor.setCharFormat(oldFormat);
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,pos);
        textedit->setTextCursor(cursor);
        textedit->blockSignals(false);
    }
}

void IDEEditor::fold(){
    int rowNum = lineNumberWidget->textCursor().blockNumber()+1;
    qDebug()<<rowNum;
    //起始行数，终止行数
    int start =0;
    int end = 0;
    //遍历查找
    for(int i=0;i<m->ListL.size();i++){
        Brackets b = m->ListL.at(i);
        qDebug()<<b.row<<"bracket";
        if(b.row == rowNum&&b.direction == 1){
            qDebug()<<"true";
            for(int j=0;j<m->ListL.size();j++){
                if(m->ListL.at(j).direction == 2&&b.match == m->ListL.at(j).match){
                    start = rowNum-1;
//                    qDebug()<<rowNum<<"start";
                    end = m->ListL.at(j).row-1;
                    break;
                }
            }
        }
    }
    //如果可以折叠
    if(start>=0&&end>=0&&end>start){
//        qDebug()<<"FOLD";
        QTextDocument * doc = textedit->document();
        QTextDocument * rowDoc = lineNumberWidget->document();
        for(int x=start+1;x<end;x++){
//            qDebug()<<x;
            QTextBlock block = doc->findBlockByNumber(x);
//            qDebug()<<block.document()->toPlainText();
            QTextBlock rowBlock = rowDoc->findBlockByNumber(x);
            //如果折叠了就展开，如果展开了就折叠
            if(rowBlock.isVisible() == true){
//                qDebug()<<"visible";
                block.setVisible(false);
                qDebug()<<block.isVisible();
                rowBlock.setVisible(false);
                textedit->viewport()->update();
                lineNumberWidget->viewport()->update();
            }
            else{
//                qDebug()<<"invisible";
                block.setVisible(true);
                rowBlock.setVisible(true);
                textedit->viewport()->update();
                lineNumberWidget->viewport()->update();
            }
            doc->adjustSize();
            rowDoc->adjustSize();
        }
    }
    lineNumberWidget->blockSignals(true);
    QTextCursor cursor = lineNumberWidget->textCursor();
    cursor.movePosition(QTextCursor::NextBlock);
    lineNumberWidget->setTextCursor(cursor);
    lineNumberWidget->blockSignals(false);
    return ;
}

void IDEEditor::setFormat(int pos,int result){
    textedit->blockSignals(true);
    textedit->selectAll();
    textedit->setCurrentCharFormat(oldFormat);
    int first = pos>result?result:pos;
    int second = result>pos?result:pos;
    //背景和选区颜色
    QTextCursor cursor = textedit->textCursor();
//    if ( cursor.anchor() )
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,first-1);
    cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
    textedit->setTextCursor(cursor);
    QTextCharFormat newformat;
    newformat.setForeground(QBrush(QColor(255,128,0)));
    cursor.mergeCharFormat(newformat);

    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,second-1);
    cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
    textedit->setTextCursor(cursor);
    cursor.mergeCharFormat(newformat);

    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,pos);
    textedit->setTextCursor(cursor);

    textedit->blockSignals(false);
}


void IDEEditor :: autoMatch(){
    prelen = length;
    length = textedit->toPlainText().length();
    if(prelen>=length){
        return ;
    }
    QString str;
    textedit->blockSignals(true);
    QTextCursor cursor = textedit->textCursor();
    int pos = cursor.position();
    cursor.movePosition(QTextCursor::PreviousCharacter,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
    textedit->setTextCursor(cursor);
    str = cursor.selectedText();
    cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::MoveAnchor);
    textedit->setTextCursor(cursor);
    if(str == "{"){
        for(int i=0;i<m->ListL.size();i++){
            if(pos == m->ListL[i].position){
                qDebug()<<m->ListL[i].match;
                if(m->ListL[i].match!=0){
                    textedit->blockSignals(false);
                    return ;
                }
            }
        }
        qDebug()<<"auto match";
        textedit->insertPlainText("}");
        transToMatching();
        cursor.movePosition(QTextCursor::PreviousCharacter);
        textedit->setTextCursor(cursor);
    }
    else if(str == "["){
        for(int i=0;i<m->ListM.size();i++){
            if(pos == m->ListM[i].position){
                qDebug()<<m->ListM[i].match;
                if(m->ListM[i].match!=0){
                    textedit->blockSignals(false);
                    return ;
                }
            }
        }
        qDebug()<<"auto match";
        textedit->insertPlainText("]");
        transToMatching();
        cursor.movePosition(QTextCursor::PreviousCharacter);
        textedit->setTextCursor(cursor);
    }
    else if(str == "("){
        for(int i=0;i<m->ListS.size();i++){
            if(pos == m->ListS[i].position){
                qDebug()<<m->ListS[i].match;
                if(m->ListS[i].match!=0){
                    textedit->blockSignals(false);
                    return ;
                }
            }
        }
        qDebug()<<"auto match";
        textedit->insertPlainText(")");
        transToMatching();
        cursor.movePosition(QTextCursor::PreviousCharacter);
        textedit->setTextCursor(cursor);
    }
    else{
        textedit->blockSignals(false);
        return ;
    }
    textedit->blockSignals(false);
    return ;
}

void IDEEditor::textchangedSlot(){
    autoMatch();
    transToMatching();
    m->matching();
    onCursorSelect();
}

void IDEEditor::cursorchangedSlot(){
}
void IDEEditor::transToMatching(){
//    qDebug()<<"transToMatching";
    QString txt = textedit->document()->toPlainText();
//    qDebug()<<txt;
    m->SetString(txt);
}
//---------------------以上为括号匹配

