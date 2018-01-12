

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
#include "Block.h"
#include "Player.h"
#include "dados.h"
#include "Leitor.h"
#include "Migalha.h"
#include "Enemy.h"
#include "Bomb.h"
#include "MegaBomb.h"
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
    QSocketNotifier *snUsr1;
    QSocketNotifier *snUsr2;
    QList<Block*> blocosdestrutiveis;
    QList<Block*> blocosindestrutiveis;
    QList<Player*> jogadoresativos;
    QList<Migalha*> migalhas;
    QList<Enemy*> inimigos;
    QList<Bomb*> bombas;
    QList<MegaBomb*> mega_bombas;
    static int sigurs1fd[2];
    static int sigurs2fd[2];
public:
    Game(string fifo, int fd);
    ~Game();
    
    static void catchusr1(int sigunused);
    static void catchusr2(int sigunused);
    static int  setup_unix_signal_handlers();
    
signals:
    void acabar();
   
public slots:
    void usr1handler(int fd);
    void usr2handler(int fd);
    void acaboulogin();
    void respostalogin(int resposta);
    void update(servcom novo);
    
    signals:
    

};





#endif /* GAME_H */

