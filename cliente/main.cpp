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



#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <pthread.h>
#include "Game.h"

void* lepipe(void*){
    
}

int main(int argc, char *argv[]) {
    pid_t pid=getpid();
    ostringstream fifoname;
    struct timeval time;
    fd_set readfds;
    time.tv_sec=300;
    time.tv_usec=0;
    fifoname << "fifo" << pid;
    if(mkfifo(fifoname.str().c_str(),S_IWUSR|S_IRUSR) != 0)
        exit(-1);
    
    QApplication app(argc, argv);
    Game novojogo(fifoname.str());
    return app.exec();
    
}
