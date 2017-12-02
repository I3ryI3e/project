#ifndef DADOS_H
#define DADOS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct clienteslogin{
    char username[30];
    char password[30];
    char fifopid[15];
}clogin;

typedef struct clientesmovimento{
    char tecla;
    char fifopid[10];
}cmov;

typedef struct enemies{
    int x, y;       // posicao no mapa
    int powerup;    // 0 = sem objeto   1 = objeto x    2 = objeto y    3 = objeto z
}enemy;

typedef struct jogadores{
    int x, y;       //posicao no mapa
    int items;      //migalhas para passar de nivel
    int bomb;
    int megabomb;
    int nvidas;     //numero de vidas do jogador
    int pontuacao;  //pontuacao do jogador
}jogador;

typedef struct tiles{
    int wall;       // 1 = parede indestrutivel   0 = sem parede   -1 = parede destrutivel
    int powerup;    // -1 = migalha   0 = sem objeto   1 = objeto x    2 = objeto y    3 = objeto z
    int explosao;   // explosao da bomba
    int personagem; // 0 = não tem  1- tem jogador  2- tem inimigo
}tile;

typedef struct servidorcomunicacao{
    tile mapa[21][31];  //campo de jogo
    jogador player;     //informacao do jogador
}servcom;

#ifdef __cplusplus
}
#endif

#endif /* DADOS_H */

