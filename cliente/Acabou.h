/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Acabou.h
 * Author: root
 *
 * Created on January 13, 2018, 2:33 PM
 */

#ifndef _ACABOU_H
#define _ACABOU_H
#include <QDialog>

#include "ui_Acabou.h"

class Acabou : public QDialog {
    Q_OBJECT
public:
    Acabou();
    virtual ~Acabou();
private:
    Ui::Acabou widget;
};

#endif /* _ACABOU_H */
