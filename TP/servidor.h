#ifndef SERVIDOR_H
#define SERVIDOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dados.h"
    
void adduser(char* nomefich, char* cmd);

void* tratateclado();

int checkcliente(char* nomefich, clicom teste);

void shutdown(int sig);

#ifdef __cplusplus
}
#endif

#endif /* SERVIDOR_H */

