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
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *pushButton;
    QLabel *label;

    void setupUi(QWidget *AStreamClass)
    {
        if (AStreamClass->objectName().isEmpty())
            AStreamClass->setObjectName(QStringLiteral("AStreamClass"));
        AStreamClass->resize(320, 331);
        label_2 = new QLabel(AStreamClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 99, 290, 61));
        label_3 = new QLabel(AStreamClass);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(0, 210, 300, 20));
        pushButton = new QPushButton(AStreamClass);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(130, 280, 60, 30));
        label = new QLabel(AStreamClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(100, 40, 64, 64));

        retranslateUi(AStreamClass);

        QMetaObject::connectSlotsByName(AStreamClass);
    } // setupUi

    void retranslateUi(QWidget *AStreamClass)
    {
        AStreamClass->setWindowTitle(QApplication::translate("AStreamClass", "AStream", 0));
        label_2->setText(QApplication::translate("AStreamClass", "FFmpeg\346\230\257\344\270\200\345\245\227\345\217\257\344\273\245\347\224\250\346\235\245\350\256\260\345\275\225\343\200\201\350\275\254\346\215\242\346\225\260\345\255\227\351\237\263\351\242\221\343\200\201\350\247\206\351\242\221\357\274\214\345\271\266\350\203\275\345\260\206\345\205\266\350\275\254\345\214\226\344\270\272\346\265\201\347\232\204\345\274\200\346\272\220\350\256\241\347\256\227\346\234\272\n"
"\347\250\213\345\272\217\343\200\202", 0));
        label_3->setText(QApplication::translate("AStreamClass", "guanfang", 0));
        pushButton->setText(QApplication::translate("AStreamClass", "PushButton", 0));
        label->setText(QApplication::translate("AStreamClass", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class AStreamClass: public Ui_AStreamClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASTREAM_H
