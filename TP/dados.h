#ifndef DADOS_H
#define DADOS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct users{
    char username[30], password[30];
}user;

typedef struct enemies{
    int x, y;   // posicao no mapa
    int obj;    // 0 = sem objeto   1 = objeto x    2 = objeto y    3 = objeto z
}enemy;

typedef struct jogadores{
    char username[30];
    int x, y;   //posicao no mapa
    pid_t pid;
}jogador;

typedef struct tiles{
    int wall;   // 1 = parede indestrutivel   0 = sem parede   -1 = parede destrutivel
}tile;

#ifdef __cplusplus
}
#endif

#endif /* DADOS_H */

