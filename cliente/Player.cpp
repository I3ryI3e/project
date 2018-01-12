//

#include "Player.h"
#include "dados.h"
#include <QKeyEvent>
#include <QBrush>
#include <QPen>
#include <sstream>
#include <unistd.h>
#include <string>
#include <fcntl.h>



Player::Player(){
    
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
//        if(pos().x() > 20 )
//            setPos(x()-20,y());
    }else if( event->key() == Qt::Key_Right || event->key() == Qt::Key_D){
        comunicacao.dados_movimento.tecla='r';
//        if(pos().x() + 20 < 640)
//            setPos(x()+20,y());
    }else if( event->key() == Qt::Key_Down || event->key() == Qt::Key_S){
        comunicacao.dados_movimento.tecla='d';
//        if(pos().y() + 20 < 640)
//            setPos(x(),y()+20);
    }else if( event->key() == Qt::Key_Up || event->key() == Qt::Key_W){
        comunicacao.dados_movimento.tecla='u';
//        if(pos().y() > 20 )
//            setPos(x(),y()-20);
    }else if( event->key() == Qt::Key_Period || event->key() == Qt::Key_1) {
        comunicacao.dados_movimento.tecla='b';
    }else if ( event ->key() == Qt::Key_Colon || event->key() == Qt::Key_Backslash){
        comunicacao.dados_movimento.tecla='m';
    }
    
    ::write(fd,&comunicacao,sizeof(comunicacao));
    ::close(fd); 
}


