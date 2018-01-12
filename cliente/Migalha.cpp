#include "Migalha.h"

Migalha::Migalha() : QGraphicsPixmapItem() {
   setPixmap(QPixmap("./migalha.jpeg").scaled(20,20,Qt::KeepAspectRatio));
}



Migalha::~Migalha() {
}

