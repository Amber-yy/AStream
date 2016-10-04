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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AStreamClass
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QPushButton *pushButton_3;

    void setupUi(QWidget *AStreamClass)
    {
        if (AStreamClass->objectName().isEmpty())
            AStreamClass->setObjectName(QStringLiteral("AStreamClass"));
        AStreamClass->resize(522, 315);
        pushButton = new QPushButton(AStreamClass);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(370, 280, 50, 25));
        pushButton_2 = new QPushButton(AStreamClass);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(440, 280, 75, 25));
        scrollArea = new QScrollArea(AStreamClass);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(0, 30, 522, 240));
        scrollArea->setStyleSheet(QStringLiteral(""));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 520, 238));
        scrollArea->setWidget(scrollAreaWidgetContents);
        pushButton_3 = new QPushButton(AStreamClass);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(499, 0, 23, 18));

        retranslateUi(AStreamClass);

        QMetaObject::connectSlotsByName(AStreamClass);
    } // setupUi

    void retranslateUi(QWidget *AStreamClass)
    {
        AStreamClass->setWindowTitle(QApplication::translate("AStreamClass", "AStream", 0));
        pushButton->setText(QApplication::translate("AStreamClass", "\350\260\203\350\211\262\346\235\277", 0));
        pushButton_2->setText(QApplication::translate("AStreamClass", "\350\207\252\345\256\232\344\271\211\347\232\256\350\202\244", 0));
        pushButton_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AStreamClass: public Ui_AStreamClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASTREAM_H
