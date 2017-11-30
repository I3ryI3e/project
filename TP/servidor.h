#ifndef SERVIDOR_H
#define SERVIDOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dados.h"
    
typedef struct infoglobal{
    clogin* clientes_activos;
    tile mapa[20][30];
    char nomefich[30];
    int continua;
}infoglobal;

    
void adduser(char* nomefich, char* cmd);

void inicializa_com(servcom* data);

void* tratateclado();

int cliente_reconhecido(char* nomefich, clogin teste);

void shutdown(int sig);

#ifdef __cplusplus
}
#endif

#endif /* SERVIDOR_H */

