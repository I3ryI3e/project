/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Game.h
 * Author: root
 *
 * Created on November 28, 2017, 4:35 PM
 */

#ifndef GAME_H
#define GAME_H
#include "Login.h"
#include <QThread>
#include  <QObject>
#include <QGraphicsScene>
#include <QDialog>
#include <QGraphicsView>
#include <string>
#include <unistd.h>
#include "dados.h"
#include "Leitor.h"
using namespace std;
class Game : public QObject{
    Q_OBJECT
    string fifoname;
    int     fdfifo;
    Login* login;
    QGraphicsScene *scene;
    QGraphicsView  *view;
    Leitor *leitor;
    QThread leitorthread;
    
    
public:
    Game(string fifo, int fd);
    ~Game();
signals:
    void acabar();
public slots:
    void acaboulogin();
    void respostalogin(int resposta);
    void update(servcom novo);
    signals:
    

};





#endif /* GAME_H */

