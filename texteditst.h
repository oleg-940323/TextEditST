#ifndef TEXTEDITST_H
#define TEXTEDITST_H

#include "linenumberwidget.h"
#include "qboxlayout.h"
#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QColumnView>
#include <QScrollArea>
#include <QPushButton>

class TextEditST: public QWidget
{
    Q_OBJECT
public:
    TextEditST(QWidget* parent = nullptr): QWidget(parent)
    {
        layout = new QHBoxLayout(this);

        // Создание кнопки-экспандера
//        QPushButton *expandButton = new QPushButton("+", this);
//        expandButton->setCheckable(true);
//        expandButton->setChecked(false);

        textEdit2 = new TextEditForST(this);
        textEdit1 = new LineNumberWidget(textEdit2, this);

        // Связывание кнопки-экспандера с функцией сворачивания и разворачивания строк
//        connect(expandButton, &QPushButton::toggled, [=](bool checked){
//            if(checked){
//                textEdit2->setMaximumHeight(textEdit2->document()->size().height());
//                expandButton->setText("-");
//            }
//            else{
//                textEdit2->setMaximumHeight(50); // Высота по умолчанию
//                expandButton->setText("+");
//            }
//        });

        layout->addWidget(textEdit1);
        //layout->addWidget(expandButton);
        layout->addWidget(textEdit2);
    }

private:
    // Объект, формирующий горизонтальный список с объектами
    QHBoxLayout *layout;

    // Текстовый редактор
    TextEditForST *textEdit2;

    // Нумерация строк
    LineNumberWidget *textEdit1;
};

#endif // TEXTEDITST_H
