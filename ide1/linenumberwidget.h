#ifndef LINENUMBERWIDGET_H
#define LINENUMBERWIDGET_H
#include <QTextEdit>
#include <QPlainTextEdit>

class LineNumberWidget : public QPlainTextEdit
{
public:
    LineNumberWidget();
public slots:
    void lineNumberChange ( int value ) ;
};

#endif // LINENUMBERWIDGET_H
