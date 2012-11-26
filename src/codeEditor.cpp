#include <QtGui>
#include "codeEditor.h"
#include "highlighter.h"


CodeEditor::CodeEditor(QWidget *parent,QTreeWidget *treeWidget) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));//one block
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect (treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(gotoErrorLine(QTreeWidgetItem*,int)));
    connect (this,SIGNAL(cursorPositionChanged()),this,SLOT(getCursorPositionChanged()));
    
    setTabStopWidth (32);
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    setEditorMenu ();

    QPalette pal = palette ();
    QColor *color = new QColor(230,230,230);
    pal.setColor (QPalette::Base,*color);
    setPalette (pal);

    new Highlighter(this->document ());
}

void CodeEditor::gotoErrorLine (QTreeWidgetItem *item, int column) {
    //jump to the error line
    int position = this->document ()->findBlockByNumber (item->text (1).toInt () - 1).position ();
    QTextCursor cur = textCursor ();
    cur.setPosition (position);
    setTextCursor (cur);
}

void CodeEditor::getCursorPositionChanged () {
    QTextCursor cur = textCursor ();
    emit emitCurrentLineNumChanged(cur.blockNumber () + 1,cur.columnNumber () + 1);
}

//void CodeEditor::inputMethodEvent (QInputMethodEvent *) {
    
//}

void CodeEditor::keyPressEvent (QKeyEvent *e) {
    QPlainTextEdit::keyPressEvent (e);
    if(e->key () == '"') {
        this->insertPlainText ("\"");
        QTextCursor c = this->textCursor ();
        c.setPosition (c.position () - 1);
        this->setTextCursor (c);
    }
    else if(e->key () == '\''){
        insertPlainText ("'");
        QTextCursor c = this->textCursor ();
        c.setPosition (c.position () - 1);
        this->setTextCursor (c);
    }
    else if(e->key () == '[') {
        insertPlainText ("]");
        QTextCursor c = this->textCursor ();
        c.setPosition (c.position () - 1);
        this->setTextCursor (c);
    }
    else if(e->key () == '{') {
        insertPlainText ("}");
        QTextCursor c = this->textCursor ();
        c.setPosition (c.position () - 1);
        this->setTextCursor (c);
    }
    else if(e->key () == '(') {
        insertPlainText (")");
        QTextCursor c = this->textCursor ();
        c.setPosition (c.position () - 1);
        this->setTextCursor (c);
    }
    else if(e->key () == '<') {
        insertPlainText (">");
        QTextCursor c = this->textCursor ();
        c.setPosition (c.position () - 1);
        this->setTextCursor (c);
        
    }
}

void CodeEditor::setEditorMenu() {
    QAction *actEditorUndo = new QAction(QIcon(":/16X16/undo.png"),tr("Undo"),this);
    actEditorUndo->setShortcut (QKeySequence::Undo);
    actEditorUndo->setDisabled (true);
    QAction *actEditorRedo = new QAction(QIcon(":/16X16/redo.png"),tr("redo"),this);
    actEditorRedo->setShortcut (QKeySequence::Redo);
    actEditorRedo->setDisabled (true);
    QAction *actEditorCopy = new QAction(QIcon(":/16X16/copy.png"),tr("Copy"),this);
    actEditorCopy->setShortcut (QKeySequence::Copy);
    actEditorCopy->setDisabled (true);
    QAction *actEditorCut = new QAction(QIcon(":/16X16/cut.png"),tr("Cut"),this);
    actEditorCut->setShortcut (QKeySequence::Cut);
    actEditorCut->setDisabled (true);
    QAction *actEditorPaste = new QAction(QIcon(":/16X16/paste.png"),tr("Paste"),this);
    actEditorPaste->setShortcut (QKeySequence::Paste);
    QAction *actEditorSelectAll = new QAction(QIcon(":/16X16/selectall.png"),tr("SelectAll"),this);
    actEditorSelectAll->setShortcut (QKeySequence::SelectAll);
    QAction *separatorOne = new QAction(this);
    QAction *separatorTwo = new QAction(this);
    separatorOne->setSeparator (true);
    separatorTwo->setSeparator (true);
    addAction (actEditorUndo);
    addAction (actEditorRedo);
    addAction (separatorOne);
    addAction (actEditorPaste);
    addAction (actEditorCopy);
    addAction (actEditorCut);
    addAction (separatorTwo);
    addAction (actEditorSelectAll);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    
    
    connect(actEditorUndo,SIGNAL(triggered()),this,SLOT(undo()));
    connect(actEditorRedo,SIGNAL(triggered()),this,SLOT(redo()));
    connect(actEditorCopy,SIGNAL(triggered()),this,SLOT(copy()));
    connect(actEditorCut,SIGNAL(triggered()),this,SLOT(cut()));
    connect(actEditorPaste,SIGNAL(triggered()),this,SLOT(paste()));
    connect(actEditorSelectAll,SIGNAL(triggered()),this,SLOT(selectAll()));
    connect (this,SIGNAL(undoAvailable(bool)),actEditorUndo,SLOT(setEnabled(bool)));
    connect (this,SIGNAL(redoAvailable(bool)),actEditorRedo,SLOT(setEnabled(bool)));
    connect(this,SIGNAL(copyAvailable(bool)),actEditorCopy,SLOT(setEnabled(bool)));
    connect(this,SIGNAL(copyAvailable(bool)),actEditorCut,SLOT(setEnabled(bool)));
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    if(digits == 1)
        digits ++;
    //above count the number of the digit character 
    //and i add that if the num <= one then i fource the space two
    
    int space = 15 + fontMetrics().width(QLatin1Char('9')) * digits;

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
        
        QColor lineColor = QColor(Qt::yellow).lighter(180);

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
            painter.setPen(Qt::black);
            //here the codeeditor margined in 13 pix,and then i draw a widget in side the margin space so if the width of this widget is small
            //than the margin size then the space i can levae in the right
            painter.drawText(0, top, lineNumberArea->width() - 10, fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
