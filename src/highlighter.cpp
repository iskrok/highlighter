#include <QtGui>

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{}

void Highlighter::highlightBlock(const QString &text)
{
    HighlightingRule rule;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    highlightingRules.clear();
    if(isC89){
        keywordPatterns << "\\bauto\\b" << "\\bbreak\\b" <<" \\bcase\\b"
                    << "\\bchar\\b" << "\\bcontinue\\b" << "\\bconst\\b"
                    << "\\bdouble\\b" << "\\bdo\\b" << "\\bdefault\\b"
                    << "\\belse\\b" << "\\benum\\b" << "\\bextern\\b"
                    << "\\bfloat\\b" << "\\bfor\\b" << "\\bgoto\\b"
                    << "\\bif\\b" << "\\bint\\b" << "\\blong\\b"
                    << "\\bregister\\b" << "\\breturn\\b" << "\\bshort\\b"
                    << "\\bsigned\\b" << "\\bsizeof\\b" << "\\bstatic\\b"
                    << "\\bstruct\\b" << "\\bswitch\\b" << "\\btypedef\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvoid\\b"
                    << "\\bvolatile\\b" << "\\bwhile\\b";
    }
    if(isC03){
        keywordPatterns << "\\bauto\\b" << "\\band\\b" << "\\band_eq\\b"
                    << "\\basm\\b" << "\\bbitand\\b" << "\\bbitor\\b"
                    << "\\bbool\\b" <<"\\bbreak\\b" <<" \\bcase\\b"
                    << "\\bcatch\\b" << "\\bchar\\b" << "\\bclass\\b"
                    << "\\bcontinue\\b" << "\\bconst\\b" << "\\bcompl\\b"
                    << "\\bconst_cast\\b" << "\\bdouble\\b" << "\\bdo\\b"
                    << "\\bdefault\\b" << "\\bdelete\\b" << "\\bdynamic_cast\\b"
                    << "\\belse\\b" << "\\benum\\b" << "\\bextern\\b"
                    << "\\bexplicit\\b" << "\\bexport\\b" << "\\bfalse\\b"
                    << "\\bfloat\\b" << "\\bfor\\b" << "\\bfriend\\b"
                    << "\\bgoto\\b" << "\\bif\\b" << "\\binline\\b"
                    << "\\bint\\b" << "\\blong\\b" << "\\bmutable\\b"
                    << "\\bnamespace\\b" << "\\bnew\\b" << "\\bnot\\b"
                    << "\\bnot_eq\\b" << "\\boperator\\b" << "\\bor\\b"
                    << "\\bor_eq\\b" << "\\bprivate\\b" << "\\bprotected\\b"
                    << "\\bpublic\\b" << "\\bregister\\b" << "\\breinterpret_cast\\b"
                    << "\\breturn\\b" << "\\bshort\\b" << "\\bsigned\\b"
                    << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstatic_cast\\b"
                    << "\\bstruct\\b" << "\\bswitch\\b" << "\\btemplate\\b"
                    << "\\bthis\\b" << "\\bthrow\\b" << "\\btrue\\b"
                    << "\\btry\\b" << "\\btypedef\\b" << "\\btypeid\\b"
                    << "\\btypename\\b" << "\\bunion\\b" << "\\bunsigned\\b"
                    << "\\busing\\b" << "\\bvirtual\\b" << "\\bvoid\\b"
                    << "\\bvolatile\\b" << "\\bwchar_t\\b" << "\\bwhile\\b"
                    << "\\bxor\\b" << "\\bxor_eq\\b";
    }
    if(isC11 || isC17){
        keywordPatterns << "\\bauto\\b" << "\\band\\b" << "\\band_eq\\b"
                    << "\\basm\\b" << "\\bbitand\\b" << "\\bbitor\\b"
                    << "\\bbool\\b" <<"\\bbreak\\b" <<" \\bcase\\b"
                    << "\\bcatch\\b" << "\\bchar\\b" << "\\bclass\\b"
                    << "\\bcontinue\\b" << "\\bconst\\b" << "\\bcompl\\b"
                    << "\\bconst_cast\\b" << "\\bdouble\\b" << "\\bdo\\b"
                    << "\\bdefault\\b" << "\\bdelete\\b" << "\\bdynamic_cast\\b"
                    << "\\belse\\b" << "\\benum\\b" << "\\bextern\\b"
                    << "\\bexplicit\\b" << "\\bexport\\b" << "\\bfalse\\b"
                    << "\\bfloat\\b" << "\\bfor\\b" << "\\bfriend\\b"
                    << "\\bgoto\\b" << "\\bif\\b" << "\\binline\\b"
                    << "\\bint\\b" << "\\blong\\b" << "\\bmutable\\b"
                    << "\\bnamespace\\b" << "\\bnew\\b" << "\\bnot\\b"
                    << "\\bnot_eq\\b" << "\\boperator\\b" << "\\bor\\b"
                    << "\\bor_eq\\b" << "\\bprivate\\b" << "\\bprotected\\b"
                    << "\\bpublic\\b" << "\\bregister\\b" << "\\breinterpret_cast\\b"
                    << "\\breturn\\b" << "\\bshort\\b" << "\\bsigned\\b"
                    << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstatic_cast\\b"
                    << "\\bstruct\\b" << "\\bswitch\\b" << "\\btemplate\\b"
                    << "\\bthis\\b" << "\\bthrow\\b" << "\\btrue\\b"
                    << "\\btry\\b" << "\\btypedef\\b" << "\\btypeid\\b"
                    << "\\btypename\\b" << "\\bunion\\b" << "\\bunsigned\\b"
                    << "\\busing\\b" << "\\bvirtual\\b" << "\\bvoid\\b"
                    << "\\bvolatile\\b" << "\\bwchar_t\\b" << "\\bwhile\\b"
                    << "\\bxor\\b" << "\\bxor_eq\\b" << "\\bchar16_t\\b"
                    << "\\bchar32_t\\b" << "\\bconstexpr\\b" << "\\bdecltype\\b"
                    << "\\bnoexcept\\b" << "\\bnullptr\\b" << "\\bstatic_assert\\b"
                    << "\\bthread_local\\b" << "\\balignas\\b" << "\\balignof\\b";
    }
    if(isC20){
        keywordPatterns << "\\bauto\\b" << "\\band\\b" << "\\band_eq\\b"
                    << "\\basm\\b" << "\\bbitand\\b" << "\\bbitor\\b"
                    << "\\bbool\\b" <<"\\bbreak\\b" <<" \\bcase\\b"
                    << "\\bcatch\\b" << "\\bchar\\b" << "\\bclass\\b"
                    << "\\bcontinue\\b" << "\\bconst\\b" << "\\bcompl\\b"
                    << "\\bconst_cast\\b" << "\\bdouble\\b" << "\\bdo\\b"
                    << "\\bdefault\\b" << "\\bdelete\\b" << "\\bdynamic_cast\\b"
                    << "\\belse\\b" << "\\benum\\b" << "\\bextern\\b"
                    << "\\bexplicit\\b" << "\\bexport\\b" << "\\bfalse\\b"
                    << "\\bfloat\\b" << "\\bfor\\b" << "\\bfriend\\b"
                    << "\\bgoto\\b" << "\\bif\\b" << "\\binline\\b"
                    << "\\bint\\b" << "\\blong\\b" << "\\bmutable\\b"
                    << "\\bnamespace\\b" << "\\bnew\\b" << "\\bnot\\b"
                    << "\\bnot_eq\\b" << "\\boperator\\b" << "\\bor\\b"
                    << "\\bor_eq\\b" << "\\bprivate\\b" << "\\bprotected\\b"
                    << "\\bpublic\\b" << "\\bregister\\b" << "\\breinterpret_cast\\b"
                    << "\\breturn\\b" << "\\bshort\\b" << "\\bsigned\\b"
                    << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstatic_cast\\b"
                    << "\\bstruct\\b" << "\\bswitch\\b" << "\\btemplate\\b"
                    << "\\bthis\\b" << "\\bthrow\\b" << "\\btrue\\b"
                    << "\\btry\\b" << "\\btypedef\\b" << "\\btypeid\\b"
                    << "\\btypename\\b" << "\\bunion\\b" << "\\bunsigned\\b"
                    << "\\busing\\b" << "\\bvirtual\\b" << "\\bvoid\\b"
                    << "\\bvolatile\\b" << "\\bwchar_t\\b" << "\\bwhile\\b"
                    << "\\bxor\\b" << "\\bxor_eq\\b" << "\\bchar16_t\\b"
                    << "\\bchar32_t\\b" << "\\bconstexpr\\b" << "\\bdecltype\\b"
                    << "\\bnoexcept\\b" << "\\bnullptr\\b" << "\\bstatic_assert\\b"
                    << "\\bthread_local\\b" << "\\bchar8_t\\b" << "\\bconcept\\b"
                    << "\\bconsteval\\b" << "\\bconstinit\\b" << "\\bco_await\\b"
                    << "\\bco_return\\b" << "\\bco_yield\\b" << "\\brequires\\b";
    }
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
    includeFormat.setForeground(Qt::darkMagenta);
    includeFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("^#include[<\"]+[A-Za-z]+[>\"]");
    rule.format = includeFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
    if(isHighlight){
        foreach (const HighlightingRule &rule, highlightingRules) {
            QRegExp expression(rule.pattern);
            int index = expression.indexIn(text);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, rule.format);
                index = expression.indexIn(text, index + length);
            }
        }
        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = commentStartExpression.indexIn(text);

        while (startIndex >= 0) {
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
}
void Highlighter::setHighligth(bool t){
    isHighlight = t;
    rehighlight();
}
void Highlighter::setC89(bool t){
    isC89 = t;
    rehighlight();
}
void Highlighter::setC03(bool t){
    isC03 = t;
    rehighlight();
}
void Highlighter::setC11(bool t){
    isC11 = t;
    rehighlight();
}
void Highlighter::setC17(bool t){
    isC17 = t;
    rehighlight();
}
void Highlighter::setC20(bool t){
    isC20 = t;
    rehighlight();
}
