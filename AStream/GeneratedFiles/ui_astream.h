/********************************************************************************
** Form generated from reading UI file 'astream.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ASTREAM_H
#define UI_ASTREAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AStreamClass
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QWidget *AStreamClass)
    {
        if (AStreamClass->objectName().isEmpty())
            AStreamClass->setObjectName(QStringLiteral("AStreamClass"));
        AStreamClass->resize(540, 35);
        label = new QLabel(AStreamClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 240, 35));
        label_2 = new QLabel(AStreamClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(245, 0, 110, 35));
        label_3 = new QLabel(AStreamClass);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(370, 0, 50, 35));
        pushButton = new QPushButton(AStreamClass);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(427, 6, 26, 23));
        pushButton_2 = new QPushButton(AStreamClass);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(467, 6, 26, 23));
        pushButton_3 = new QPushButton(AStreamClass);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(507, 6, 26, 23));

        retranslateUi(AStreamClass);

        QMetaObject::connectSlotsByName(AStreamClass);
    } // setupUi

    void retranslateUi(QWidget *AStreamClass)
    {
        AStreamClass->setWindowTitle(QApplication::translate("AStreamClass", "AStream", 0));
        label->setText(QApplication::translate("AStreamClass", "\345\223\252\344\275\215\350\266\205\347\272\247\345\244\247\346\230\216\346\230\237\347\232\204\346\255\214\345\220\215\345\225\212\357\274\214\346\255\214\345\220\215\345\225\212\357\274\214\350\277\231\344\271\210\351\225\277", 0));
        label_2->setText(QApplication::translate("AStreamClass", "\346\255\214\346\211\213\345\220\215\350\277\231\344\271\210\351\225\277\345\244\237\344\272\206\345\225\212", 0));
        label_3->setText(QApplication::translate("AStreamClass", "00:00", 0));
        pushButton->setText(QApplication::translate("AStreamClass", "PushButton", 0));
        pushButton_2->setText(QApplication::translate("AStreamClass", "PushButton", 0));
        pushButton_3->setText(QApplication::translate("AStreamClass", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class AStreamClass: public Ui_AStreamClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASTREAM_H
