/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Leitor.cpp
 * Author: root
 * 
 * Created on December 2, 2017, 10:02 AM
 */

#include "Leitor.h"




Leitor::Leitor(int fd):fifo(fd), ajogar(0){
}

void Leitor::preparaleitura(){
    loop=new QEventLoop();
    notifier=new QSocketNotifier(fifo,QSocketNotifier::Read,this);
    connect(notifier,SIGNAL(activated(int)),this,SLOT(ler(int)));
    
    
    loop->exec();
}

void Leitor::ler(int fd){
    servcom novo;
    int resposta=0;
    
        if(ajogar){
            read(fifo,&novo,sizeof(novo));
            emit updaterecebido(novo);
        }else{
            if(read(fifo,&resposta,sizeof(resposta)) < 0)
                printf("Erro a ler!");
            emit respostaLogin(resposta);
            if(resposta == 1)
                ajogar=1;
        }
}

void Leitor::acabar(){
    loop->exit(0);
    emit finished();
    return;
}
Leitor::~Leitor(){
    ::close(fifo);
    delete notifier;
    delete loop;
}