#ifndef _WIN_H
#define _WIN_H

#include "ui_Win.h"

class Win : public QDialog {
    Q_OBJECT
public:
    Win();
    virtual ~Win();
private:
    Ui::Win widget;
};

#endif /* _WIN_H */
