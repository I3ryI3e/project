

#ifndef LEITOR_H
#define LEITOR_H
#include <QObject>
#include "dados.h"
#include <QSocketNotifier>
#include <qt4/QtCore/qeventloop.h>
#include <unistd.h>

class Leitor : public QObject{
    Q_OBJECT
    int fifo;
    int ajogar;
    QSocketNotifier* notifier;
    QEventLoop* loop;
    
public:
    Leitor(int fd);
    ~Leitor();
public slots:
        void acabar();
        void ler(int fd);
        void preparaleitura();
signals:
    void finished();
    void respostaLogin(int resposta);
    void updaterecebido(servcom movimento);

};

#endif /* LEITOR_H */

