#ifndef TEXTBLOCKCOLLAPSER_H
#define TEXTBLOCKCOLLAPSER_H

#include "qtextobject.h"
#include <QPlainTextEdit>

class TextBlockCollapser : public QObject
{
    Q_OBJECT
public:
    explicit TextBlockCollapser(QPlainTextEdit* textEdit, QObject* parent = nullptr) : QObject(parent), m_textEdit(textEdit)
        {
            connect(m_textEdit, &QPlainTextEdit::cursorPositionChanged, this, &TextBlockCollapser::updateCollapseState);
        }

        void toggleBlockCollapse()
        {
            // получаем курсор для текущей позиции
            QTextCursor cursor = m_textEdit->textCursor();

            // получаем текущий блок
            QTextBlock block = cursor.block();

            // получаем следующий блок
            QTextBlock nextBlock = block.next();

            // получаем текущий уровень отступа
            int currentIndentation = block.blockFormat().indent();

            while (nextBlock.isValid() && nextBlock.blockFormat().indent() > currentIndentation) {
                // сворачиваем или разворачиваем блок
                bool isCollapsed = nextBlock.isVisible();
                QTextCursor nextCursor(nextBlock);
                nextCursor.movePosition(QTextCursor::StartOfBlock);
                nextBlock.setVisible(!isCollapsed);
                m_textEdit->setTextCursor(nextCursor);

                // переходим к следующему блоку
                nextBlock = nextBlock.next();
            }
        }

        void updateCollapseState()
     {
            // получаем текущий блок
            QTextCursor cursor = m_textEdit->textCursor();
            QTextBlock block = cursor.block();

            // получаем следующий блок
            QTextBlock nextBlock = block.next();

            // получаем текущий уровень отступа
            int currentIndentation = block.blockFormat().indent();

            // проверяем, доступно ли сворачивание блоков
            bool canCollapse = false;

            while (nextBlock.isValid() && nextBlock.blockFormat().indent() > currentIndentation) {
                canCollapse = true;
                nextBlock = nextBlock.next();
            }

            // устанавливаем доступность кнопки сворачивания
            emit canCollapseBlocksChanged(canCollapse);
        }

    signals:
        void canCollapseBlocksChanged(bool canCollapse);

    private:
        QPlainTextEdit* m_textEdit;
};

#endif // TEXTBLOCKCOLLAPSER_H
