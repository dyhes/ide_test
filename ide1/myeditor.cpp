#include "myeditor.h"
#include<QTextEdit>
#include<QDebug>
myEditor::myEditor(QWidget *parent): QPlainTextEdit(parent)
{
    lighter=new Highlighter(this->document());
    connect(this,myEditor::cursorPositionChanged,this,myEditor::currentLineHighlighter);
}
void myEditor::currentLineHighlighter()
{

    QList<QTextEdit::ExtraSelection> extraSelections;
//    qDebug()<<"line changed";

        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);


    this->setExtraSelections(extraSelections);

}
