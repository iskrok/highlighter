#include <QtGui>

#include "codeeditor.h"
#include "qmenu.h"
#include "qpushbutton.h"
#include <QDialog>
#include <QTextBlock>
#include <QLineEdit>
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);
    lineColor = QColor(Qt::yellow).lighter(160);
    isLineNumbers = true;
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}



int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}



void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}



void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}



void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}



void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}



void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();

    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            if(!isLineNumbers)
                painter.setPen(Qt::lightGray);
            else
                painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                    Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }

}
void CodeEditor::findtxt(){
    QDialog* diag = new QDialog(this);
    diag->setWindowTitle("Find");
    QLineEdit* line = new QLineEdit(diag);
    QPushButton *btn = new QPushButton("Find",diag);
    btn->move(125,0);
    diag->show();
    connect(line,SIGNAL(textEdited(QString)),this,SLOT(getText(QString)));
    connect(btn,SIGNAL(clicked()),this,SLOT(findText()));
}
void CodeEditor::getText(QString a){
    tmp = a;
}
void CodeEditor::getText2(QString a){
    tmp2 = a;
}
void CodeEditor::findText(){
    QTextDocument* doc = new QTextDocument(this);
    doc = this->document();
    QTextCursor cur = doc->find(tmp);
    if(!cur.isNull())
        setTextCursor(cur);
}
void CodeEditor::findrepl(){
    QDialog* diag = new QDialog(this);
    diag->setWindowTitle("Find and Replace");
    QLineEdit* line = new QLineEdit(diag);
    QLineEdit* line2 = new QLineEdit(diag);
    QPushButton *btn = new QPushButton("Find",diag);
    QPushButton *btn2 = new QPushButton("Replace",diag);
    btn->move(125,0);
    btn2->move(125,25);
    line2->move(0,25);
    diag->show();
    connect(line,SIGNAL(textEdited(QString)),this,SLOT(getText(QString)));
    connect(line2,SIGNAL(textEdited(QString)),this,SLOT(getText2(QString)));
    connect(btn,SIGNAL(clicked()),this,SLOT(findText()));
    connect(btn2,SIGNAL(clicked()),this,SLOT(textRepl()));
}
void CodeEditor::textRepl(){
    QTextDocument* doc = new QTextDocument(this);
    doc = this->document();
    QTextCursor cur = doc->find(tmp);
    if(!cur.isNull()){
        cur.insertText(tmp2);
    }
}
void CodeEditor::setWrap(bool tmp){
    if (tmp)
        setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    else
        setWordWrapMode(QTextOption::NoWrap) ;
}
void CodeEditor::contextMenuEvent(QContextMenuEvent *event){
    QMenu *menu = createStandardContextMenu();
    QTextCursor cur = this->textCursor();
    setTextCursor(cur);
    QAction *a;
    a = new QAction(tr("&Select"), this);
    connect(a,SIGNAL(triggered()),this,SLOT(select_word()));
    if(cur.selectedText() != ""){
        a->setDisabled(true);
    }
    menu->addAction(a);
    a = new QAction(tr("&Select Line"), this);
    menu->addAction(a);
    connect(a,SIGNAL(triggered()),this,SLOT(select_line()));
    if(cur.selectedText() != ""){
        a->setDisabled(true);
    }
    QList<QAction*> l;
    l = menu->actions();
    menu->removeAction(l[l.size()-3]);
    menu->removeAction(l[l.size()-4]);
    menu->exec(event->globalPos());
    delete menu;
}
void CodeEditor::select_word(){
    QTextCursor cur = this->textCursor();
    cur.select(QTextCursor::WordUnderCursor);
    setTextCursor(cur);
}
void CodeEditor::select_line(){
    QTextCursor cur = this->textCursor();
    cur.select(QTextCursor::LineUnderCursor);
    setTextCursor(cur);
}
void CodeEditor::setLineColor(QColor col){
    lineColor = col;
}
void CodeEditor::setLineNumbers(bool t){
    isLineNumbers = t;
    update();
}
QColor CodeEditor::getLineColor(){
    return lineColor;
}
