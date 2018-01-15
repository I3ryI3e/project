#include "PowerUp.h"

PowerUp::PowerUp() : QGraphicsPixmapItem(){
    setPixmap(QPixmap("./power.png").scaled(20,20,Qt::KeepAspectRatio));
}

PowerUp::~PowerUp() {
}

