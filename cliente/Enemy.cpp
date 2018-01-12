/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Enemy.cpp
 * Author: root
 * 
 * Created on January 12, 2018, 12:00 PM
 */

#include "Enemy.h"

Enemy::Enemy() : QGraphicsPixmapItem(){  
    setPixmap(QPixmap("./enemy.jpeg").scaled(20,20,Qt::KeepAspectRatio));
}


Enemy::~Enemy() {
}

