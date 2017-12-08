

#ifndef _LOGIN_H
#define _LOGIN_H

#include "ui_Login.h"
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sstream>
using namespace std;

class Login : public QDialog {
    Q_OBJECT
public:
    Login();
    virtual ~Login();
public slots:
    Ui::Login getWidget();
private:
    Ui::Login widget;
};

#endif /* _LOGIN_H */
