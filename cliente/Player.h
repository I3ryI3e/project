/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Player.h
 * Author: root
 *
 * Created on December 6, 2017, 3:49 PM
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsRectItem>
#include <QObject>

class Player : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
   void keyPressEvent(QKeyEvent* event) override;
   public slots:
       void spawn();
private:

};

#endif /* PLAYER_H */

