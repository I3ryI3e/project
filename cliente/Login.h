/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Login.h
 * Author: root
 *
 * Created on October 28, 2017, 2:57 PM
 */

#ifndef _LOGIN_H
#define _LOGIN_H

#include "ui_Login.h"
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sstream>
using namespace std;

class Login : public QDialog {
    Q_OBJECT
public:
    Login();
    virtual ~Login();
public slots:
    Ui::Login getWidget();
private:
    Ui::Login widget;
};

#endif /* _LOGIN_H */
