
#include "Enemy.h"

Enemy::Enemy() : QGraphicsPixmapItem(){  
    setPixmap(QPixmap("./enemy.jpeg").scaled(20,20,Qt::KeepAspectRatio));
}


Enemy::~Enemy() {
}

