#include "linenumberwidget.h"
#include<QTextEdit>
#include <qdebug.h>
#include <QScrollBar>
LineNumberWidget::LineNumberWidget()
{

}

void LineNumberWidget::lineNumberChange(int value)
{
    qDebug ( ) << value ;
    QScrollBar *bar = verticalScrollBar ( ) ;
    bar->setValue(value);
}
