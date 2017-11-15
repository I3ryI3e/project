/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: root
 *
 * Created on October 28, 2017, 2:48 PM
 */


#include "Login.h"

int main(int argc, char *argv[]) {
    pid_t pid=getpid();
    ostringstream fifoname;
    fifoname << "fifo" << pid;
    if(mkfifo(fifoname.str().c_str(),S_IWUSR|S_IRUSR) != 0)
        exit(-1);

    QApplication app(argc, argv);
    Login first(fifoname.str());
    first.show();
    return app.exec();
}
