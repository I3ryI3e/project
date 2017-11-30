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
#include  <QObject>
#include <QGraphicsScene>
#include <QDialog>
#include <QGraphicsView>
#include <string>
#include <unistd.h>
#include "dados.h"
#include <pthread.h>
#include <signal.h>
using namespace std;


class Game : public QObject{
    Q_OBJECT
    Login* login;
    QGraphicsScene *scene;
    QGraphicsView  *view;
    string fifoname;

public:
    Game(string fifo);
    void update(servcom novo);
    ~Game();

public slots:
    void acaboulogin();

};


typedef struct infoglobal{
    Game* jogo;
    pthread_t threadid;
    string fifo;
}informacao;


#endif /* GAME_H */

