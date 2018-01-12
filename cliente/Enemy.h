/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Enemy.h
 * Author: root
 *
 * Created on January 12, 2018, 12:00 PM
 */

#ifndef ENEMY_H
#define ENEMY_H
#include <QGraphicsPixmapItem>
#include <qt4/QtGui/qgraphicsscene.h>

class Enemy : public QGraphicsPixmapItem{
public:
    Enemy();
    virtual ~Enemy();
private:

};

#endif /* ENEMY_H */

