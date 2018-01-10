

#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Player();
   void keyPressEvent(QKeyEvent* event) override;
   public slots:
private:

};

#endif /* PLAYER_H */

