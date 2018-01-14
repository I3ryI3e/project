

#include "Player.h"
#include "dados.h"
#include <QKeyEvent>
#include <QBrush>
#include <QPen>
#include <sstream>
#include <unistd.h>
#include <string>
#include <fcntl.h>



Player::Player(): QGraphicsPixmapItem(){
}

void Player::keyPressEvent(QKeyEvent* event){
    c_cmov comunicacao;
    int fd;
    ostringstream fifoname; 

    fifoname << "/tmp/fifo" << getpid();
    
    if((fd= (::open("/tmp/fifoserv", O_WRONLY)))< 0){
        return;
    }
    comunicacao.tipo_mensagem=2;
    strcpy(comunicacao.dados_movimento.fifopid,fifoname.str().c_str());
    
    if( event->key() == Qt::Key_Left || event->key() == Qt::Key_A){
        comunicacao.dados_movimento.tecla='l';
    }else if( event->key() == Qt::Key_Right || event->key() == Qt::Key_D){
        comunicacao.dados_movimento.tecla='r';
    }else if( event->key() == Qt::Key_Down || event->key() == Qt::Key_S){
        comunicacao.dados_movimento.tecla='d';
    }else if( event->key() == Qt::Key_Up || event->key() == Qt::Key_W){
        comunicacao.dados_movimento.tecla='u';
    }else if( event->key() == Qt::Key_Period || event->key() == Qt::Key_1) {
        comunicacao.dados_movimento.tecla='b';
    }else if ( event ->key() == Qt::Key_Comma || event->key() == Qt::Key_Backslash){
        comunicacao.dados_movimento.tecla='m';
    }
    
    ::write(fd,&comunicacao,sizeof(comunicacao));
    ::close(fd); 
}


