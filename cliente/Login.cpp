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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>




Login::Login(string fifo): fifoname(fifo){
    widget.setupUi(this);
    widget.submitbutton->hide();
    widget.passwordtext->setEchoMode(QLineEdit::Password);
    show();
}

void Login::on_submitbutton_clicked(){
    clogin novo;
    int fd,tipomsg;
    widget.usernametext->text().data();
    strcpy(novo.username,widget.usernametext->text().toLatin1().data());
    strcpy(novo.password,widget.passwordtext->text().toLatin1().data());
    strcpy(novo.fifopid,fifoname.c_str());
    
    if((fd= (::open("/tmp/fifoserv", O_WRONLY)))< 0){
        return;
    }
    tipomsg=1;
    write(fd,&tipomsg,sizeof(tipomsg));
    write(fd,&novo,sizeof(novo));
    ::close(fd);           
}
Ui::Login Login::getWidget(){
    return widget;
}

Login::~Login() {
}
