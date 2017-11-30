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
#include <QAbstractButton>

extern  informacao info;

Game::Game(string fifo):fifoname(fifo){
    login=new Login(fifo);
    scene = new QGraphicsScene();
    view = new QGraphicsView();
    QObject::connect(login->getWidget().submitbutton,SIGNAL(released()),this,SLOT(acaboulogin()));
}

void Game::acaboulogin(){
    login->hide();
    view->show();
    view->setFixedSize(800, 600);
    scene->setSceneRect(0, 0, 800, 600);
}
void Game::update(servcom novo){
    
}

Game::~Game(){
    delete login;
    delete scene;
    delete view;
    pthread_kill(info.threadid,SIGUSR1);
    pthread_join(info.threadid,NULL);
    unlink(fifoname.c_str());
}


