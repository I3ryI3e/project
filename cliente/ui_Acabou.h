/********************************************************************************
** Form generated from reading UI file 'Acabou.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACABOU_H
#define UI_ACABOU_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Acabou
{
public:
    QTextEdit *textEdit;
    QPushButton *pushButton;

    void setupUi(QDialog *Acabou)
    {
        if (Acabou->objectName().isEmpty())
            Acabou->setObjectName(QString::fromUtf8("Acabou"));
        Acabou->resize(973, 317);
        textEdit = new QTextEdit(Acabou);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(280, 80, 321, 78));
        QPalette palette;
        QBrush brush(QColor(185, 185, 185, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        QBrush brush1(QColor(247, 247, 247, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        textEdit->setPalette(palette);
        pushButton = new QPushButton(Acabou);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(390, 250, 90, 28));

        retranslateUi(Acabou);

        QMetaObject::connectSlotsByName(Acabou);
    } // setupUi

    void retranslateUi(QDialog *Acabou)
    {
        Acabou->setWindowTitle(QApplication::translate("Acabou", "Acabou", 0, QApplication::UnicodeUTF8));
        textEdit->setHtml(QApplication::translate("Acabou", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Cantarell'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600;\">O jogo acabou</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Acabou", "Sair", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Acabou: public Ui_Acabou {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACABOU_H
