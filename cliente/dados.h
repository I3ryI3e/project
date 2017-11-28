#ifndef DADOS_H
#define DADOS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct threadlepipecliente{
    char fifopid[10];

}tlepipe;    
    
typedef struct clientescomunicacao{
    char username[30];
    char password[30];
    char tecla;     //tecla clicada
    char  fifopid[10];
}clicom;

typedef struct enemies{
    int x, y;       // posicao no mapa
    int powerup;    // 0 = sem objeto   1 = objeto x    2 = objeto y    3 = objeto z
}enemy;

typedef struct jogadores{
    int x, y;       //posicao no mapa
    int speed;      //velocidade do jogador
    int rangeexp;   //tamanho da explosao
    int items;      //migalhas para passar de nivel
    int bomb;
    int megabomb;
    int nvidas;     //numero de vidas do jogador
    int pontuacao;  //pontuacao do jogador
}jogador;

typedef struct tiles{
    int wall;       // 1 = parede indestrutivel   0 = sem parede   -1 = parede destrutivel
    int powerup;    // 0 = sem objeto   1 = objeto x    2 = objeto y    3 = objeto z
    int explosao;   // explosao da bomba
    int inimigo;    // tem ou nao tem 1 ou 0
    int jogador;    // tem ou nao tem 1 ou 0
}tile;

typedef struct servidorcomunicacao{
    tile mapa[20][30];  //campo de jogo
    jogador player;     //informacao do jogador
}servcom;

#ifdef __cplusplus
}
#endif

#endif /* DADOS_H */

