
#include "Game.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <signal.h>


int Game::sigurs1fd[] = {0,0};
int Game::sigurs2fd[] = {0,0};

Game::Game(string fifo, int fd): fifoname(fifo), fdfifo(fd) ,login(new Login()), scene(new QGraphicsScene()), view(new QGraphicsView()){
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
    
    ::socketpair(AF_UNIX,SOCK_STREAM,0,sigurs1fd);
    ::socketpair(AF_UNIX,SOCK_STREAM,0,sigurs2fd);
    
    snUsr1 = new QSocketNotifier(sigurs1fd[1],QSocketNotifier::Read,this);
    connect(snUsr1,SIGNAL(activated(int)),this, SLOT(usr1handler(int)));
    snUsr2 = new QSocketNotifier(sigurs2fd[1],QSocketNotifier::Read,this);
    connect(snUsr2,SIGNAL(activated(int)),this,SLOT(usr2handler(int)));
    setup_unix_signal_handlers();
    
    
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
    clogin novo;
    int fd,tipomsg;
    login->getWidget().usernametext->text().data();
    strcpy(novo.username,login->getWidget().usernametext->text().toLatin1().data());
    strcpy(novo.password,login->getWidget().passwordtext->text().toLatin1().data());
    strcpy(novo.fifopid,fifoname.c_str());
    
    if((fd= (::open("/tmp/fifoserv", O_WRONLY)))< 0){
        return;
    }
    tipomsg=1;
    ::write(fd,&tipomsg,sizeof(tipomsg));
    ::write(fd,&novo,sizeof(novo));
    ::close(fd); 
    leitorthread.start();

}
void Game::respostalogin(int resposta){
    if(resposta){
        login->hide();
        delete login;
        login=nullptr;
        view->show();
        view->setFixedSize(800, 600);
        scene->setSceneRect(0, 0, 800, 600);
    }
    else if(resposta == 0){
        login->hide();
        delete login;
        login=new Login();
        login->show();
        connect(login->getWidget().submitbutton,SIGNAL(clicked()),this,SLOT(acaboulogin()));
    }else{
        QCoreApplication::quit();
    }
}
void Game::update(servcom novo){
    
}

Game::~Game(){
    delete login;
    delete scene;
    delete view;
    emit acabar();
    leitorthread.quit();
    unlink(fifoname.c_str());
}


