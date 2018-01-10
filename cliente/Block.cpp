#include "Block.h"
#include "dados.h"
#include "QPen"
#include "QBrush"

Block::Block(bool edestrutivel) : destrutivel(edestrutivel){
    setRect(0, 0, 20, 20);
    if (destrutivel) {
	setPen(QPen(Qt::gray));
	setBrush(QBrush(Qt::gray));
    } else {
	setBrush(QBrush(Qt::darkGray));
	setPen(QPen(Qt::black));
    }
}


Block::~Block() {
}
