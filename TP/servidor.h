#ifndef SERVIDOR_H
#define SERVIDOR_H

#ifdef __cplusplus
extern "C" {
#endif

#define LIN 31
#define COL 31
    
typedef struct clienteslogin{
    char username[30];
    char password[30];
    char fifopid[15];
}clogin;

typedef struct clientesmovimento{
    char tecla;         // |u = UP| |d = DOWN| |l = LEFT| |r = RIGHT| |b = BOMB| |m = MEGABOMB|
    char fifopid[15];
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
    int wall;       // -1 = migalha   0 = sem parede   1 = parede indestrutivel   2 = parede destrutivel
    int powerup;    // 0 = sem objeto   1 = objeto x    2 = objeto y    3 = objeto z ...
    int explosao;   // 0 = sem explosao   1 = explosao (da bomba)
    int personagem; // -1 = um inimigo   0 = nada   1 = um jogador
}tile;

typedef struct servidorcomunicacao{
    int estado;             // 0 = jogo a decorrer   1 = acabou   2 = kick
    tile mapa[LIN][COL];    //campo de jogo
    jogador player;         //informacao do jogador
}servcom;

typedef struct dadoscliente{
    jogador atributos_cli;
    clogin dados_cli;
}dcli;

typedef struct infoglobal{
    dcli* clientes_activos;
    int cli_activos;
    tile mapa[LIN][COL];
    char nomefich[30];
    int continua;
}infoglobal;
    
void adduser(char* nomefich, char* cmd);

void kickplayer_ativo(char* cmd);

int addnewcliente_ativo(clogin newcli, int nmaxplay);

void set_atributos_newcli();

void re_nascimento(int num_cli);

void set_struct_tocliente(servcom* data, char fpid[15]);

void inicializa_mapa(int n_migalhas);

void updownleftrigth(int x, int y, int n_cli);

void trataevalida_tecla(cmov movcli);

void* tratateclado();

int cliente_reconhecido(char* nomefich, clogin teste);

void thread_sigusr2(int sig);

void shutdown_sigusr1(int sig);

#ifdef __cplusplus
}
#endif

#endif /* SERVIDOR_H */
