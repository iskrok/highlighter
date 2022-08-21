#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>
#include <QRegExp>
class QTextDocument;

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);
public slots:
    void setHighligth(bool);
    void setC89(bool);
    void setC03(bool);
    void setC11(bool);
    void setC17(bool);
    void setC20(bool);
protected:
    void highlightBlock(const QString &text);

private:
    bool isHighlight = true;
    bool isC89 = true;
    bool isC03 = false;
    bool isC11 = false;
    bool isC17 = false;
    bool isC20 = false;
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat includeFormat;
};

#endif
