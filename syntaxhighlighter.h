#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QPlainTextEdit>
#include <QTextDocument>

class SyntaxHighlighter : public QSyntaxHighlighter
{
public:
    SyntaxHighlighter(QTextDocument* parent = nullptr) : QSyntaxHighlighter(parent)
    {
        // Операторы
        QTextCharFormat keywordFormat;
        keywordFormat.setFontWeight(QFont::Bold);
        keywordFormat.setFontCapitalization(QFont::AllUppercase);
        keywordFormat.setForeground(Qt::blue);

        // Текст в кавычках
        QTextCharFormat quotesFormat;
        quotesFormat.setForeground(Qt::darkYellow);

        // Простой текст
        identifierFormat.setForeground(Qt::black);

        // Цифры
        QTextCharFormat numberFormat;
        numberFormat.setFontItalic(true);
        numberFormat.setForeground(Qt::magenta);

        // Коментарии
        commentFormat.setForeground(Qt::darkGreen);

        // Ключевые слова
        QStringList statementPatterns;
        statementPatterns << "(?i)\\bdiv\\b" << "(?i)\\bmod\\b"
                        << "(?i)\\band\\b"<< "(?i)\\bor\\b"<< "(?i)\\bxor\\b"<< "(?i)\\bnot\\b"
                        << "(?i)\\bif\\b" << "(?i)\\bthen\\b"<< "(?i)\\belseif\\b" << "(?i)\\belse\\b"<< "(?i)\\bend_if\\b"
                        << "(?i)\\bfor\\b" << "(?i)\\bto\\b" << "(?i)\\bby\\b" << "(?i)\\bdo\\b" << "(?i)\\bend_for\\b"
                        << "(?i)\\bwhile\\b" << "(?i)\\bend_while\\b"
                        << "(?i)\\bcase\\b" << "(?i)\\bof\\b"  << "(?i)\\bend_case\\b"
                        << "(?i)\\band_bit\\b"<< "(?i)\\bor_bit\\b"<< "(?i)\\bxor_bit\\b"<< "(?i)\\bnot_bit\\b"<< "(?i)\\bset_bit\\b"<< "(?i)\\breset_bit\\b"
                        << "(?i)\\btime\\b"<< "(?i)\\bPROGRAM\\b"<< "(?i)\\bVAR\\b"<< "(?i)\\bCONSTANT\\b"<< "(?i)\\bEND_VAR\\b"
                            << "(?i)\\bVAR_IN_OUT\\b"<< "(?i)\\bVAR_INPUT\\b"<< "(?i)\\bVAR_OUTPUT\\b"<< "(?i)\\bFUNCTION_BLOCK\\b"
                        << "(?i)\\bbool\\b"<< "(?i)\\bbyte\\b"<< "(?i)\\bint\\b"<< "(?i)\\buint\\b"<< "(?i)\\bsint\\b"<< "(?i)\\busint\\b"<< "(?i)\\bdint\\b"<< "(?i)\\budint\\b"<< "(?i)\\blint\\b"<< "(?i)\\bulint\\b"
                        << "(?i)\\bword\\b"<< "(?i)\\breal\\b"<< "(?i)\\blreal\\b"<< "(?i)\\b__UXINT\\b"<< "(?i)\\b__XINT\\b"<< "(?i)\\b__XWORD\\b"<< "(?i)\\bARRAY\\b"<< "(?i)\\bbit\\b"<< "(?i)\\bblWORD\\b"
                        << "(?i)\\bDATE\\b"<< "(?i)\\bDATE_AND_TIME\\b"<< "(?i)\\blDATE_AND_TIME\\b"<< "(?i)\\blDATE\\b"<< "(?i)\\bdt\\b"<< "(?i)\\bDWORD\\b"<< "(?i)\\bldt\\b"<< "(?i)\\btime_of_day\\b"<< "(?i)\\bltime\\b"
                        << "(?i)\\bltime_of_day\\b"<< "(?i)\\bltod\\b"<< "(?i)\\bmax_element\\b"<< "(?i)\\bpointer\\b"<< "(?i)\\bstring\\b"<< "(?i)\\btod\\b"<< "(?i)\\bwstring\\b"
                        << "(?i)\\brepeat\\b" << "(?i)\\bexit\\b"<< "(?i)\\bCONTINUE\\b";

        // Методы
        QStringList methodPatterns;
        methodPatterns << "(?i)\\bconcat\\b"<< "(?i)\\bleft\\b"<< "(?i)\\bright\\b"<< "(?i)\\bmid\\b";


        for (const QString& pattern : qAsConst(statementPatterns))
        {
            QRegularExpression expression(pattern);
            highlightingRules.append({ expression, keywordFormat});
        }

        for (const QString& pattern : qAsConst(methodPatterns))
        {
            QRegularExpression expression(pattern);
            highlightingRules.append({ expression, keywordFormat});
        }

        // Изменение цвета цифр в пурпурный
        QRegularExpression numberExpression("\\b[0-9]+\\b");
        highlightingRules.append({ numberExpression, numberFormat});

        // Изменение цвета текста, заключенного в кавычки, в желтый (кавычки тоже)
        QRegularExpression stringExpression("\".*\"");
        highlightingRules.append({ stringExpression, quotesFormat});

        // Изменение цвета текста, начинающегося с символа "//" и заканчивающегося символом новой строки.
        QRegularExpression commentExpression("//[^\n]*");
        highlightingRules.append({ commentExpression, commentFormat});

       commentStartExpression = QRegularExpression(QStringLiteral("\\(\\*"));
       //highlightingRules.append({ commentStartExpression, commentFormat});

       commentEndExpression = QRegularExpression(QStringLiteral("\\*\\)"));
       //highlightingRules.append({ commentEndExpression, commentFormat});
    }

    // Изменение стиля текста
    void setFontStyle(const QFont* font)
    {
        for (HighlightingRule& rule : highlightingRules)
            rule.format.setFont(*font);
    }

protected:
    void highlightBlock(const QString& text) override
    {

        checkRegularExpression(text);

        setCurrentBlockState(0);

        int startIndex = 0;

        if (previousBlockState() != 1)
            startIndex = text.indexOf(commentStartExpression);

        while (startIndex >= 0)
        {
            QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
            int endIndex = match.capturedStart();
            int commentLength = 0;
            if (endIndex == -1)
            {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            }
            else
                commentLength = endIndex - startIndex + match.capturedLength();

            setFormat(startIndex, commentLength, commentFormat);
            startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
        }
    }

private:

    QTextCharFormat commentFormat, identifierFormat;
    QRegularExpressionMatchIterator iterator;
    QRegularExpressionMatch match;
    QRegularExpression commentEndExpression, commentStartExpression;

    struct HighlightingRule
    {
        // Регулярное выражение
        QRegularExpression pattern;

        // Формат текста
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    // Проверка строки на наличие регулярных выражений
    void checkRegularExpression(const QString &text)
    {
        for (HighlightingRule& rule : highlightingRules)
        {
            iterator = rule.pattern.globalMatch(text);

            while (iterator.hasNext())
            {
                match = iterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
    }
};
