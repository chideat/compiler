#include <QtGui>
#include "highlighter.h"

//! [0]
Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+[ ]*(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    macroFormat.setForeground (Qt::darkBlue);
    rule.pattern =  QRegExp("#[ ]*(include|define|if|ifndef|ifdef|else|elif|endif)");
    rule.format = macroFormat;
    highlightingRules.append (rule);

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\b([A-Za-z_][A-Za-z0-9_]*[ ]*)((?=[:{&])|(?=[A-Za-z_][A-Za-z0-9_]*.*))\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    

    
    dataTypeFormat.setForeground (Qt::darkMagenta);
    dataTypeFormat.setFontWeight (QFont::Bold);
    QStringList dataTypeList;
    dataTypeList << "\\bauto\\b" << "\\bdouble\\b" << "\\bconst\\b" << "\\bint\\b" 
                 << "\\blong\\b" << "\\benum\\b" << "\\bfloat\\b" << "\\bchar\\b"
                << "\\bregister\\b" << "\\bshort\\b" << "\\bsigned\\b" << "\\bstatic\\b" << "\\bunion\\b" << "\\bextern\\b"
                << "\\bunsigned\\b" << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bstruct\\b";
    foreach (const QString &pattern, dataTypeList) {
        rule.pattern = QRegExp(pattern);
        rule.format = dataTypeFormat;
        highlightingRules.append(rule);
    }
    
    otherTypeFormat.setForeground(Qt::darkRed);
    otherTypeFormat.setFontWeight(QFont::Bold);
    QStringList otherKeywordList;
    otherKeywordList << "\\bbreak\\b" << "\\bcase\\b" << "\\bcontinue\\b" << "\\bdefault\\b"
        << "\\bdo\\b" << "\\belse\\b" << "\\bfor\\b" << "\\bgoto\\b" << "\\bif\\b" << "\\breturn\\b"
        << "\\bsizeof\\b" << "\\bstruct\\b" << "\\bswitch\\b" << "\\btypedef\\b" << "\\bwhile\\b"
        << "\\bNULL\\b"<<"\\bthis\\b";
    foreach (const QString &pattern, otherKeywordList) {
        rule.pattern = QRegExp(pattern);
        rule.format = otherTypeFormat;
        highlightingRules.append(rule);
    }
    
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("(\".*\")|(\'.*\')|(<.*>)");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
    
    multiLineCommentFormat.setForeground(Qt::blue);
    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}
//! [6]

//! [7]
void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
//! [7] //! [8]
    setCurrentBlockState(0);
//! [8]

//! [9]
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

//! [9] //! [10]
    while (startIndex >= 0) {
//! [10] //! [11]
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
//! [11]
