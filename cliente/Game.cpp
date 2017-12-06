/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Game.cpp
 * Author: root
 * 
 * Created on November 28, 2017, 4:35 PM
 */

#include "Game.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>






Game::Game(string fifo, int fd): fifoname(fifo), fdfifo(fd) ,login(new Login()), scene(new QGraphicsScene()), view(new QGraphicsView()){
    leitor = new Leitor(fdfifo);
    leitor->moveToThread(&leitorthread);
    
    
    connect(login->getWidget().submitbutton,SIGNAL(clicked()),this,SLOT(acaboulogin()));
    
    connect(leitor, SIGNAL(updaterecebido(servcom)), this, SLOT(update(servcom)));
    connect(this, SIGNAL(acabar()),leitor,SLOT(acabar()));
    connect(leitor, SIGNAL(respostaLogin(int)),this,SLOT(respostalogin(int)));
    connect(&leitorthread, SIGNAL(started()), leitor, SLOT(preparaleitura()));
    connect(leitor, SIGNAL(finished()), &leitorthread, SLOT(quit()));
    connect(leitor, SIGNAL(finished()), leitor, SLOT(deleteLater()));
    connect(&leitorthread, SIGNAL(finished()), &leitorthread, SLOT(deleteLater()));
}

void Game::acaboulogin(){
    clogin novo;
    int fd,tipomsg;
    login->getWidget().usernametext->text().data();
    strcpy(novo.username,login->getWidget().usernametext->text().toLatin1().data());
    strcpy(novo.password,login->getWidget().passwordtext->text().toLatin1().data());
    strcpy(novo.fifopid,fifoname.c_str());
    
    if((fd= (::open("/tmp/fifoserv", O_WRONLY)))< 0){
        return;
    }
    tipomsg=1;
    ::write(fd,&tipomsg,sizeof(tipomsg));
    ::write(fd,&novo,sizeof(novo));
    ::close(fd); 
    leitorthread.start();

}
void Game::respostalogin(int resposta){
    if(resposta){
        login->hide();
        delete login;
        login=nullptr;
        view->show();
        view->setFixedSize(800, 600);
        scene->setSceneRect(0, 0, 800, 600);
    }
    else if(resposta == 0){
        login->hide();
        delete login;
        login=new Login();
        login->show();
        connect(login->getWidget().submitbutton,SIGNAL(clicked()),this,SLOT(acaboulogin()));
    }else{
        QCoreApplication::quit();
    }
}
void Game::update(servcom novo){
    
}

Game::~Game(){
    delete login;
    delete scene;
    delete view;
    emit acabar();
    leitorthread.quit();
    unlink(fifoname.c_str());
}


