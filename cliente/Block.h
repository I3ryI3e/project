#ifndef BLOCK_H
#define BLOCK_H
#include "QGraphicsRectItem"

class Block: public QGraphicsRectItem {
    bool destrutivel;
public:
    Block(bool edestrutivel);
    virtual ~Block();
private:

};

#endif /* BLOCK_H */

