/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: root
 *
 * Created on October 28, 2017, 2:48 PM
 */



#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <pthread.h>
#include <iostream>
#include "Game.h"
#include <signal.h>



informacao info;

void catchsignal(int a){
    pthread_exit(NULL);
}

void* lepipe(void* nomefifo){
    int fd;
    servcom novo;
    signal(SIGUSR1,catchsignal);
    char* fifo=(char*)nomefifo;
    if((fd=::open(fifo,O_RDWR)) < 0){
        cout << "Erro a abrir o ficheiro";
        pthread_exit(NULL);
    }
    while(1){
        ::read(fd,&novo,sizeof(novo));
        info.jogo->update(novo);        
    }
    
    
}

int main(int argc, char *argv[]) {
    pid_t pid=getpid();
    ostringstream fifoname;   

    fifoname << "/tmp/fifo" << pid;
    if(mkfifo(fifoname.str().c_str(),S_IWUSR|S_IRUSR) != 0)
        exit(-1);
    QApplication app(argc, argv);
    Game novojogo(fifoname.str());
    info.jogo=&novojogo;
    return app.exec();
    
}
