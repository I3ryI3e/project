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

Game::Game(string nomefifo): fifoname(nomefifo){
    login=new Login(fifoname);
    scene = new QGraphicsScene();
    view = new QGraphicsView();
    QObject::connect(login,SIGNAL(on_submitbutton_realeased()),this,SLOT(acaboulogin()));
}

void Game::acaboulogin(){
    login->hide();
    view->show();
    view->setFixedSize(800, 600);
    scene->setSceneRect(0, 0, 800, 600);
}

Game::~Game(){
    delete login;
    delete scene;
    delete view;
    unlink(fifoname.c_str());
}


