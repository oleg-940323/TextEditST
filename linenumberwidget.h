#include "qpainter.h"
#include "qtextobject.h"
#include "texteditforst.h"
#include <QFrame>
#include <QBoxLayout>
#include <QPlainTextEdit>
#include <QLabel>

class LineNumberWidget : public QWidget
{

public:
    LineNumberWidget(TextEditForST* editor, QWidget* parent = nullptr) : QWidget(parent)
    {
        m_editor = editor;

        // Установка фиксированной ширины для виджета
        setFixedWidth(fontMetrics().QFontMetrics::horizontalAdvance(QStringLiteral("9")) + 10);

        // Установка шрифта
        setFont(editor->font);

        // Установка соединений сигналов и слотов
        connect(m_editor, &QPlainTextEdit::blockCountChanged, this, &LineNumberWidget::update);
        connect(m_editor, &QPlainTextEdit::updateRequest, this, &LineNumberWidget::update);
        connect(m_editor, &QPlainTextEdit::cursorPositionChanged, this, &LineNumberWidget::highlightCurrentLine);

        // Выполнение первичного обновления
        update();
    }

protected:
    // Переопределение события отрисовки
    void paintEvent(QPaintEvent* event) override
    {
        QPainter painter(this);
        painter.fillRect(event->rect(), Qt::lightGray);


        QTextBlock block = m_editor->getFirstVisibleBlock();
        int blockNumber = block.blockNumber();
        int top = (int)m_editor->getBlockBoundingGeometry(block).translated(m_editor->getContentOffset()).top();
        int bottom = top + (int)m_editor->getBlockBoundingRect(block).height();

        while (block.isValid() && top <= event->rect().bottom())
        {
            if (block.isVisible() && bottom >= event->rect().top())
            {
                QString number = QString::number(blockNumber + 1);
                painter.setPen(Qt::black);
                painter.drawText(0, top + 1, width(), fontMetrics().height(), Qt::AlignRight, number);
            }

            block = block.next();
            top = bottom;
            bottom = top + (int)m_editor->getBlockBoundingRect(block).height();
            ++blockNumber;
        }
    }

private slots:
    // Обновление виджета
    void update()
    {
        // Вызов перерисовки виджета
        QRect rect = m_editor->viewport()->rect();
        rect.setWidth(width());
        QWidget::update(rect);

        // текст с номером последней строки
        QString lastRowNumberText = QString::number(m_editor->document()->lastBlock().blockNumber() + 1);

        // объект QFontMetrics для текущего шрифта виджета
        QFontMetrics fontMetrics(font());

        // ширина текста с номером последней строки
        int textWidth = fontMetrics.QFontMetrics::horizontalAdvance(lastRowNumberText);

        // установка фиксированной ширины виджета равной ширине текста
        setFixedWidth(textWidth + 2);
    }

    // Подсветка текущей строки
    void highlightCurrentLine()
    {
        QList<QTextEdit::ExtraSelection> selections;

        if (!m_editor->isReadOnly())
        {
            QTextEdit::ExtraSelection selection;

            QColor lineColor = QColor(Qt::yellow).lighter(160);

            selection.format.setBackground(lineColor);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = m_editor->textCursor();
            selection.cursor.clearSelection();

            selections.append(selection);
        }

        m_editor->setExtraSelections(selections);
    }

private:
    TextEditForST* m_editor;
};
