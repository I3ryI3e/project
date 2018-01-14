#include "Explosion.h"

Explosion::Explosion(int type) : QGraphicsPixmapItem(){
    if(type == 0){  //centro explosao
        setPixmap(QPixmap("./expcenter.png").scaled(20,20,Qt::KeepAspectRatio));
    }else if(type == 1){ //explosao horizontal
        setPixmap(QPixmap("./exphorizontal.png").scaled(20,20,Qt::KeepAspectRatio));
    }else{  //explosao vertical
        setPixmap(QPixmap("./expvertical.png").scaled(20,20,Qt::KeepAspectRatio));
    }
}


Explosion::~Explosion() {
}

