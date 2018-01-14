/********************************************************************************
** Form generated from reading UI file 'Login.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QLabel *welcome;
    QLabel *loginlabel;
    QLabel *usernamelabel;
    QLabel *passwordlabel;
    QLineEdit *usernametext;
    QLineEdit *passwordtext;
    QPushButton *submitbutton;
    QTextEdit *errortext;

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QString::fromUtf8("Login"));
        Login->resize(800, 400);
        Login->setAutoFillBackground(true);
        welcome = new QLabel(Login);
        welcome->setObjectName(QString::fromUtf8("welcome"));
        welcome->setGeometry(QRect(310, 30, 180, 30));
        QFont font;
        font.setBold(true);
        font.setUnderline(true);
        font.setWeight(75);
        welcome->setFont(font);
        loginlabel = new QLabel(Login);
        loginlabel->setObjectName(QString::fromUtf8("loginlabel"));
        loginlabel->setGeometry(QRect(160, 80, 51, 20));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        loginlabel->setFont(font1);
        usernamelabel = new QLabel(Login);
        usernamelabel->setObjectName(QString::fromUtf8("usernamelabel"));
        usernamelabel->setGeometry(QRect(50, 120, 71, 20));
        passwordlabel = new QLabel(Login);
        passwordlabel->setObjectName(QString::fromUtf8("passwordlabel"));
        passwordlabel->setGeometry(QRect(50, 180, 71, 20));
        usernametext = new QLineEdit(Login);
        usernametext->setObjectName(QString::fromUtf8("usernametext"));
        usernametext->setGeometry(QRect(140, 110, 201, 41));
        passwordtext = new QLineEdit(Login);
        passwordtext->setObjectName(QString::fromUtf8("passwordtext"));
        passwordtext->setGeometry(QRect(140, 170, 201, 41));
        submitbutton = new QPushButton(Login);
        submitbutton->setObjectName(QString::fromUtf8("submitbutton"));
        submitbutton->setGeometry(QRect(229, 240, 111, 41));
        errortext = new QTextEdit(Login);
        errortext->setObjectName(QString::fromUtf8("errortext"));
        errortext->setGeometry(QRect(400, 130, 341, 61));
        errortext->setAutoFillBackground(true);
        errortext->setStyleSheet(QString::fromUtf8("background-color: rgb(204, 204, 204);"));
        errortext->setFrameShape(QFrame::NoFrame);
        errortext->setReadOnly(true);
        errortext->setTextInteractionFlags(Qt::NoTextInteraction);

        retranslateUi(Login);
        QObject::connect(passwordtext, SIGNAL(textChanged(QString)), submitbutton, SLOT(show()));
        QObject::connect(submitbutton, SIGNAL(clicked()), errortext, SLOT(hide()));

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QDialog *Login)
    {
        Login->setWindowTitle(QApplication::translate("Login", "Login", 0, QApplication::UnicodeUTF8));
        welcome->setText(QApplication::translate("Login", "Bem vindo ao SOBomber!", 0, QApplication::UnicodeUTF8));
        loginlabel->setText(QApplication::translate("Login", "Login:", 0, QApplication::UnicodeUTF8));
        usernamelabel->setText(QApplication::translate("Login", "Username:", 0, QApplication::UnicodeUTF8));
        passwordlabel->setText(QApplication::translate("Login", "Password:", 0, QApplication::UnicodeUTF8));
        submitbutton->setText(QApplication::translate("Login", "Submit", 0, QApplication::UnicodeUTF8));
        errortext->setHtml(QApplication::translate("Login", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Cantarell'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:600; color:#da2808;\">Error! Username/Password combination not accepted</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
