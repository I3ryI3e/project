#include "Bomb.h"

Bomb::Bomb() : QGraphicsPixmapItem(){
    setPixmap(QPixmap("./Bombe.png").scaled(20,20,Qt::KeepAspectRatio));
}


Bomb::~Bomb() {
}

