/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MegaBomb.cpp
 * Author: root
 * 
 * Created on January 12, 2018, 12:31 PM
 */

#include "MegaBomb.h"

MegaBomb::MegaBomb() : QGraphicsPixmapItem(){
    setPixmap(QPixmap("MBombe.png").scaled(20,20,Qt::KeepAspectRatio));
}


MegaBomb::~MegaBomb() {
}

