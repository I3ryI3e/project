/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Player.cpp
 * Author: root
 * 
 * Created on December 6, 2017, 3:49 PM
 */

#include "Player.h"

void Player::keyPressEvent(QKeyEvent* event){
    if( event->key() == Qt::Key_Left){
        if(pos().x() > 0 )
            setPos(x()-10,y());
    }else if( event->key() == Qt::Key_Right){
        if(pos().x() + 100 < 800)
            setPos(x()+10,y())
    }else if( event->key() == Qt::Key_Down){
        if(pos().y() + 100 < 600)
            setPos(x(),y()+10);
    }else if( event->key() == Qt::Key_Up){
        if(pos().y() > 0 )
            setPos(x(),y()-10);
    }
}


