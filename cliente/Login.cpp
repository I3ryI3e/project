/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Login.cpp
 * Author: root
 *
 * Created on October 28, 2017, 2:57 PM
 */

#include "Login.h"
#include <QString>
#include <string>
#include "dados.h"
#include <string>




Login::Login(){
    widget.setupUi(this);
    widget.submitbutton->hide();
    widget.passwordtext->setEchoMode(QLineEdit::Password);
    show();
}


Ui::Login Login::getWidget(){
    return widget;
}

Login::~Login() {
}
