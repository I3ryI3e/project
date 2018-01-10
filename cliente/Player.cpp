

#include "Player.h"
#include "dados.h"
#include <QKeyEvent>
#include <QBrush>
#include <QPen>


Player::Player(){
    setPixmap(QPixmap("./BomberManAzul0.jpeg").scaled(20,20,Qt::KeepAspectRatio));
}

void Player::keyPressEvent(QKeyEvent* event){
    if( event->key() == Qt::Key_Left){
        if(pos().x() > 20 )
            setPos(x()-20,y());
    }else if( event->key() == Qt::Key_Right){
        if(pos().x() + 20 < 440)
            setPos(x()+20,y());
    }else if( event->key() == Qt::Key_Down){
        if(pos().y() + 20 < 640)
            setPos(x(),y()+20);
    }else if( event->key() == Qt::Key_Up){
        if(pos().y() > 20 )
            setPos(x(),y()-20);
    }
}


