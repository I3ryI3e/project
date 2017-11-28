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

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QString::fromUtf8("Login"));
        Login->resize(800, 400);
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

        retranslateUi(Login);
        QObject::connect(passwordtext, SIGNAL(textChanged(QString)), submitbutton, SLOT(show()));

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
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
