#include "syntaxhighlighter.h"

class TextEditForST : public QPlainTextEdit
{
public:
    QFont font;

    TextEditForST(QWidget *parent = nullptr)  : QPlainTextEdit(parent)
    {
        installEventFilter(this);
        setTabStopDistance(40);
        setLineWrapMode(LineWrapMode::NoWrap);
        highlighter = new SyntaxHighlighter(document());

        connect(this, &TextEditForST::textChanged, this, &TextEditForST::getLine);

        // Формируем шрифт
        font.setFamily("Time New Roman");
        font.setPointSize(12);

        // Устанавливаем шрифт виджету
        setFont(font);
        highlighter->setFontStyle(&font);
    }

    QTextBlock getFirstVisibleBlock()
    {
        return firstVisibleBlock();
    }

    QRectF getBlockBoundingRect(QTextBlock block)
    {
        return blockBoundingRect(block);
    }

    QRectF getBlockBoundingGeometry(QTextBlock block)
    {
        return blockBoundingGeometry(block);
    }

    QPointF getContentOffset()
    {
        return contentOffset();
    }

private:
    int index = 0;
    QTextCursor cursor;
    bool delTab, tabFlag;
    QString text, originalText;
    SyntaxHighlighter* highlighter;
    QList<QString> listLeft = {"end_case", "end_for", "end_if", "end_var"};
    QList<QString> listRight = {"repeat", "until", "do", "of", "then", "else", "var constant", "var",  "var_in_out", "var_input", "var_output"};

    // Добавление таба
    bool addTab(int  cntTab)
    {
        // Перебираем ключевые слова для добавления табуляции
        for (auto & str : qAsConst(listRight))
        {
            if  (text.endsWith(str, Qt::CaseInsensitive))
            {
               // Получаем текст текущей строки
               originalText = document()->findBlockByNumber(index).text();

               tabFlag = true;

               for (int i = 0; i <= cntTab; i++)
                    originalText.prepend("\t");

               // Вставляем новую строку
               cursor.select(QTextCursor::LineUnderCursor);
               cursor.insertText(originalText);
               cursor.movePosition(QTextCursor::EndOfLine);
               return true;
            }
        }
        return false;
    }

    // Удаление таба
    bool deleteTab(int cntTab)
    {
        // Перебираем ключевые слова для удаления табуляции
        for (auto & str : qAsConst(listLeft))
            if  (text.endsWith(str, Qt::CaseInsensitive) || text.endsWith(str + ";", Qt::CaseInsensitive))
            {
                // Получаем текст текущей строки
                originalText = document()->findBlockByNumber(index).text();

                // Дабавляем необходимое число табов
                if (cntTab != 0)
                    for (int i = 0; i < cntTab - 1; i++)
                         originalText.prepend("\t");

                tabFlag = true;

                // Вставляем новую строку
                cursor.select(QTextCursor::LineUnderCursor);
                cursor.insertText(originalText);
                cursor.movePosition(QTextCursor::EndOfLine);
                return true;
            }
        return false;
    }

    // Поддержание строк на одном уровне табов
    void checkSymbol(int cntTab)
    {
        // Получаем текст текущей строки
        originalText = document()->findBlockByNumber(index).text();

        for (int i = 0; i < cntTab; i++)
             originalText.prepend("\t");

        tabFlag = true;

        // Вставляем новую строку
        cursor.select(QTextCursor::LineUnderCursor);
        cursor.insertText(originalText);
        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.setPosition(cursor.position() + cntTab);
        setTextCursor(cursor);
    }
protected:

    // Получение значения нажатой клавиши
    bool eventFilter(QObject *obj, QEvent *event)
        {
            if (event->type() == QEvent::KeyPress)
            {
                int pos;
                QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

                // Нажали ли Backspace
                if (keyEvent->key() == Qt::Key_Backspace)
                {
                    // Получаем текущую позицию курсора
                    cursor = textCursor();

                    // Получаем текст предыдущей строки
                    text = document()->findBlockByNumber(cursor.blockNumber()).text();

                    pos = cursor.positionInBlock();

                    if (pos > 0  && text.at(pos - 1) == "\t")
                        delTab = true;
                }
            }
            return QPlainTextEdit::eventFilter(obj, event);
        }

public slots:
    void getLine()
    {
        // Количество табов в строке
        int cntTab = 0;

        // Весь текст редактора
        QString allText;

        // Получаем индекс текущей строки
        index = textCursor().blockNumber();

        // Первая ли эта строка
        if (index == 0)
            return;

        // Был ли подставлен или удален символ табуляции
        if (tabFlag)
        {
            tabFlag = false;
            return;
        }

        // Получаем текущую позицию курсора
        cursor = textCursor();

        // Получаем текст предыдущей строки
        text = document()->findBlockByNumber(index - 1).text();

        // Подсчет табов в начале строки
        for (int i = 0; i < text.length(); i++)
            if (text[i] == '\t')
                cntTab++;
            else
                break;

        // получаем символ на позиции курсора
        allText = toPlainText();

        if (allText.at(cursor.position() - 1) != "\n")
            return;

        text = text.trimmed();

        // Проверка строки на добавление таба
        if (addTab(cntTab))
            return;

        // Проверка строки на удаление таба
        if (deleteTab(cntTab))
            return;

        if (delTab)
            delTab = false;
        else
            checkSymbol(cntTab);
    }
};
