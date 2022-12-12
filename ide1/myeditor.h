#ifndef MYEDITOR_H
#define MYEDITOR_H
#include <QPlainTextEdit>
#include<myhighlighter.h>

class myEditor:public QPlainTextEdit
{
public:
    myEditor(QWidget *parent = 0);
    Highlighter *lighter;

public slots:
   void currentLineHighlighter();
};

#endif // MYEDITOR_H
