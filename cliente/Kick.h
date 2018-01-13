/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Kick.h
 * Author: root
 *
 * Created on January 13, 2018, 2:27 PM
 */

#ifndef _KICK_H
#define _KICK_H
#include <QDialog>

#include "ui_Kick.h"

class Kick : public QDialog {
    Q_OBJECT
public:
    Kick();
    virtual ~Kick();
private:
    Ui::Kick widget;
};

#endif /* _KICK_H */
