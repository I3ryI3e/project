#ifndef SERVIDOR_H
#define SERVIDOR_H

#ifdef __cplusplus
extern "C" {
#endif

void adduser(char* nomefich, char* cmd);

void tratateclado();

void shutdown(int sig);

#ifdef __cplusplus
}
#endif

#endif /* SERVIDOR_H */

