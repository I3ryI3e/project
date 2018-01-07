#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include "Game.h"
#include <signal.h>


int main(int argc, char *argv[]) {
    pid_t pid=getpid();
    int fdfifo;
    ostringstream fifoname; 

    fifoname << "/tmp/fifo" << pid;
    if((mkfifo(fifoname.str().c_str(),S_IWUSR|S_IRUSR)) != 0)
        return 0;
    if((fdfifo=::open(fifoname.str().c_str(),O_RDWR)) == -1){
        cout << "Erro a abrir o fifo";
        return 0;
    }
    QApplication app(argc, argv);
    qRegisterMetaType<servcom>("servcom");
    Game novojogo(fifoname.str(),fdfifo);
    return app.exec();
}
