
#include "Game.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <signal.h>
#include <QGraphicsTextItem>
#include "Explosion.h"



int Game::sigurs1fd[] = {0,0};
int Game::sigurs2fd[] = {0,0};

Game::Game(string fifo, int fd): fifoname(fifo), fdfifo(fd) ,login(new Login()), scene(new QGraphicsScene()), view(new QGraphicsView()), estado(-1){
    leitor = new Leitor(fdfifo);
    leitor->moveToThread(&leitorthread);
    
    connect(login->getWidget().submitbutton,SIGNAL(clicked()),this,SLOT(acaboulogin()));
    
    connect(leitor, SIGNAL(updaterecebido(servcom)), this, SLOT(update(servcom)));
    connect(this, SIGNAL(acabar()),leitor,SLOT(acabar()));
    connect(leitor, SIGNAL(respostaLogin(int)),this,SLOT(respostalogin(int)));
    connect(&leitorthread, SIGNAL(started()), leitor, SLOT(preparaleitura()));
    connect(leitor, SIGNAL(finished()), &leitorthread, SLOT(quit()));
    connect(leitor, SIGNAL(finished()), leitor, SLOT(deleteLater()));
    connect(&leitorthread, SIGNAL(finished()), &leitorthread, SLOT(deleteLater()));
    connect(qApp,SIGNAL(aboutToQuit()),this,SLOT(quit()));
    
    ::socketpair(AF_UNIX,SOCK_STREAM,0,sigurs1fd);
    ::socketpair(AF_UNIX,SOCK_STREAM,0,sigurs2fd);
    
    snUsr1 = new QSocketNotifier(sigurs1fd[1],QSocketNotifier::Read,this);
    connect(snUsr1,SIGNAL(activated(int)),this, SLOT(usr1handler(int)));
    snUsr2 = new QSocketNotifier(sigurs2fd[1],QSocketNotifier::Read,this);
    connect(snUsr2,SIGNAL(activated(int)),this,SLOT(usr2handler(int)));
    setup_unix_signal_handlers();
    
    
}

void Game::quit() {
    int fd;
    c_cmov acaba;
    acaba.tipo_mensagem=3;
    strcpy(acaba.dados_movimento.fifopid,fifoname.c_str());
    if(estado == 1){
        if((fd = (::open("/tmp/fifoserv", O_WRONLY)))< 0){
            return;
        }
        ::write(fd,&acaba,sizeof(c_cmov));
        close(fd);
    }
}

int Game::setup_unix_signal_handlers() {
    
    struct sigaction usr1,usr2;
    

    usr1.sa_handler = Game::catchusr1;
    sigemptyset(&usr1.sa_mask);
    usr1.sa_flags = SA_RESTART;

    if (sigaction(SIGUSR1, &usr1,NULL))
       return 1;
    
    
    usr2.sa_handler = Game::catchusr2;
    sigemptyset(&usr2.sa_mask);
    usr2.sa_flags = SA_RESTART;

    if (sigaction(SIGUSR2, &usr2, NULL))
       return 1;
    return 0;
}
void Game::catchusr1(int sigunused){
    char a= 1;
    ::write(Game::sigurs1fd[0], &a, sizeof(a));
}
void Game::catchusr2(int sigunused){
    char a=1;
    ::write(Game::sigurs2fd[0],&a,sizeof(a));
}
void Game::usr1handler(int fd){
    snUsr1->setEnabled(false);
    char tmp;
    ::read(Game::sigurs1fd[1], &tmp, sizeof(tmp));
    delete login;
    delete scene;
    delete view;
    emit acabar();
    leitorthread.quit();
    unlink(fifoname.c_str());
    QCoreApplication::quit();
    
}
void Game::usr2handler(int fd){
    snUsr2->setEnabled(false);
    char tmp;
    ::read(Game::sigurs2fd[1], &tmp, sizeof(tmp));
    delete login;
    delete scene;
    delete view;
    emit acabar();
    leitorthread.quit();
    unlink(fifoname.c_str());
    QCoreApplication::quit();
}


void Game::acaboulogin(){
    c_clogin novo;
    int fd;
    novo.tipo_mensagem=1;
    login->getWidget().usernametext->text().data();
    strcpy(novo.dados_login.username,login->getWidget().usernametext->text().toLatin1().data());
    strcpy(novo.dados_login.password,login->getWidget().passwordtext->text().toLatin1().data());
    strcpy(novo.dados_login.fifopid,fifoname.c_str());
    
    if((fd= (::open("/tmp/fifoserv", O_WRONLY)))< 0){
        return;
    }
    ::write(fd,&novo,sizeof(novo));
    ::close(fd); 
    leitorthread.start();

}
void Game::respostalogin(int resposta){
    if(resposta){
        login->hide();
        delete login;
        login=nullptr;
        scene->setSceneRect(0, 0, 1024, 800);
        view->setScene(scene);
        view->setFixedSize(1024, 800);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->show();
        estado = 1;
    }
    else if(resposta == 0){
        login->getWidget().errortext->show();
        login->getWidget().usernametext->clear();
        login->getWidget().passwordtext->clear();
        estado = -1;
    }else{
        QCoreApplication::quit();
    }
}
void Game::update(servcom novo){
    
    if(novo.estado == 1){
        blocosdestrutiveis.clear();
        blocosindestrutiveis.clear();
        migalhas.clear();
        jogadoresativos.clear();
        qDeleteAll(scene->items());
        Block* novo_bloco;
        Player* novo_player;
        Migalha* nova_migalha;
        QGraphicsTextItem* dados;
        Enemy* novo_inimigo;
        Bomb* nova_bomba;
        MegaBomb* nova_mega;
        Explosion* nova_explosao;
        for(int i=0;i<=N_LIN+1;i++){
            for(int j=0;j<=N_COL+1;j++){
                if(i==0 || i == (N_LIN+1)){
                    novo_bloco= new Block(false);
                    novo_bloco->setPos(i*20,j*20);
                    blocosindestrutiveis.append(novo_bloco);
                    scene->addItem(novo_bloco);
                }else if(j==0 || j==(N_COL+1)){
                    novo_bloco= new Block(false);
                    novo_bloco->setPos(i*20,j*20);
                    blocosindestrutiveis.append(novo_bloco);
                    scene->addItem(novo_bloco);
                }
            }
        }
        for(int i=0;i<N_LIN;i++){
            for(int j=0;j<N_COL;j++){
                if(novo.mapa[i][j].wall == 2){
                    novo_bloco= new Block(true);
                    novo_bloco->setPos(((i*20)+20),((j*20)+20));
                    blocosdestrutiveis.append(novo_bloco);
                    scene->addItem(novo_bloco);
                }else if(novo.mapa[i][j].wall == 1){
                    novo_bloco= new Block(false);
                    novo_bloco->setPos(((i*20)+20),((j*20)+20));
                    blocosindestrutiveis.append(novo_bloco);
                    scene->addItem(novo_bloco);
                }else if (novo.mapa[i][j].wall == 4){
                    nova_bomba= new Bomb();
                    nova_bomba->setPos(((i*20)+20),((j*20)+20));
                    bombas.append(nova_bomba);
                    scene->addItem(nova_bomba);
                }else if(novo.mapa[i][j].wall == 5){
                    nova_mega= new MegaBomb();
                    nova_mega->setPos(((i*20)+20),((j*20)+20));
                    mega_bombas.append(nova_mega);
                    scene->addItem(nova_mega);
                }else if(novo.mapa[i][j].personagem == -1){
                    novo_inimigo= new Enemy();
                    novo_inimigo->setPos(((i*20)+20),((j*20)+20));
                    inimigos.append(novo_inimigo);
                    scene->addItem(novo_inimigo);
                }else if (novo.mapa[i][j].personagem == 1){
                    if(i != novo.player.x || j != novo.player.y){
                        novo_player = new Player();
                        novo_player->setPixmap(QPixmap("./aliado.png").scaled(20,20,Qt::KeepAspectRatio));
                        novo_player->setPos(((i*20)+20),((j*20)+20));
                        scene->addItem(novo_player);
                        jogadoresativos.append(novo_player);
                    }
                }else if(novo.mapa[i][j].explosao == 1){
                    nova_explosao= new Explosion(0);
                    nova_explosao->setPos(((i*20)+20),((j*20)+20));
                    scene->addItem(nova_explosao);
                }else if(novo.mapa[i][j].explosao == 2){
                    nova_explosao= new Explosion(1);
                    nova_explosao->setPos(((i*20)+20),((j*20)+20));
                    scene->addItem(nova_explosao);
                }else if(novo.mapa[i][j].explosao == 3){
                    nova_explosao= new Explosion(2);
                    nova_explosao->setPos(((i*20)+20),((j*20)+20));
                    scene->addItem(nova_explosao);
                }else if(novo.mapa[i][j].wall == 3){
                    nova_migalha= new Migalha();
                    nova_migalha->setPos(((i*20)+20),((j*20)+20));
                    migalhas.append(nova_migalha);
                    scene->addItem(nova_migalha);
                  }
            }
        }
        novo_player = new Player();
        novo_player->setPixmap(QPixmap("./jogador.png").scaled(20,20,Qt::KeepAspectRatio));
        novo_player->setPos((novo.player.x*20+20),(novo.player.y*20+20));
        jogadoresativos.append(novo_player);
        novo_player->setFlag(QGraphicsItem::ItemIsFocusable);
        novo_player->setFocus();
        scene->addItem(novo_player);
        dados= new QGraphicsTextItem();
        dados->setPlainText(QString("Bombas: ") + QString::number(novo.player.bomb));
        dados->setDefaultTextColor(Qt::blue);
        dados->setFont(QFont("times",14));
        dados->setPos(690,0);
        scene->addItem(dados);
        dados= new QGraphicsTextItem();
        dados->setPlainText(QString("Mega Bombas: ") + QString::number(novo.player.megabomb));
        dados->setDefaultTextColor(Qt::red);
        dados->setFont(QFont("times",14));
        dados->setPos(690,30);
        scene->addItem(dados);
        dados= new QGraphicsTextItem();
        dados->setPlainText(QString("Vidas: ") + QString::number(novo.player.nvidas));
        dados->setDefaultTextColor(Qt::green);
        dados->setFont(QFont("times",14));
        dados->setPos(690,60);
        scene->addItem(dados);
        dados= new QGraphicsTextItem();
        dados->setPlainText(QString("Score: ") + QString::number(novo.player.pontuacao));
        dados->setDefaultTextColor(Qt::black);
        dados->setFont(QFont("times",14));
        dados->setPos(690,90);
        scene->addItem(dados);
    }else if(novo.estado == 2){
            view->hide();
            janela_kick= new Kick();
            janela_kick->show();
        }else if(novo.estado == 0){
                view->hide();
                janela_acabou = new Acabou();
                janela_acabou->show();
            }else if(novo.estado == 5){
                view->hide();
                janela_win = new Win();
                janela_win->show();
            }


}

Game::~Game(){
    if(login!=nullptr) 
        delete login;
    blocosdestrutiveis.clear();
    blocosindestrutiveis.clear();
    migalhas.clear();
    jogadoresativos.clear();
    inimigos.clear();
    bombas.clear();
    mega_bombas.clear();
    qDeleteAll(scene->items());
    delete scene;
    delete view;
    emit acabar();
    leitorthread.quit();
    unlink(fifoname.c_str());
}


