

#include "Player.h"
#include <QKeyEvent>
#include <QBrush>
#include <QPen>


Player::Player(){
    setRect(0, 0, 20, 20);
    
	setPen(QPen(Qt::blue));
	setBrush(QBrush(Qt::blue));
}

void Player::keyPressEvent(QKeyEvent* event){
    if( event->key() == Qt::Key_Left){
        if(pos().x() > 0 )
            setPos(x()-10,y());
    }else if( event->key() == Qt::Key_Right){
        if(pos().x() + 100 < 800)
            setPos(x()+10,y());
    }else if( event->key() == Qt::Key_Down){
        if(pos().y() + 100 < 600)
            setPos(x(),y()+10);
    }else if( event->key() == Qt::Key_Up){
        if(pos().y() > 0 )
            setPos(x(),y()-10);
    }
}


