#include "MegaBomb.h"

MegaBomb::MegaBomb() : QGraphicsPixmapItem(){
    setPixmap(QPixmap("./MBombe.png").scaled(20,20,Qt::KeepAspectRatio));
}


MegaBomb::~MegaBomb() {
}

