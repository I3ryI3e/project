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

typedef struct thread_trata_cmd{
    int estado_thread;
    char nomefich[30];
}trata_cmd;

typedef struct thread_bomb_mega_bomb{
    int range;
    int x, y;
}bomb_mb;

typedef struct bomba_com{
    int bomb_type;              // 4 = explode bomba   5 = acaba bomba   6 = explode m_bomba   7 = acaba m_bomba
    bomb_mb bomba;
}bomb_com;

typedef struct clientesmovimento{
    char tecla;     // |u = UP| |d = DOWN| |l = LEFT| |r = RIGHT| |b = BOMB| |m = MEGABOMB|
    char fifopid[15];
}cmov;

typedef struct comunicacao_cmov{
    int tipo_mensagem; // 
    cmov dados_movimento;
}c_cmov;

typedef struct enemies{
    int vida;       // 0 = morto   1 = vivo
    int id;         // numero do inimigo
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
    int wall;       // 0 = sem parede   1 = parede indestrutivel   2 = parede destrutivel   3 = migalha   4 = bomba   5 = megabomba
    int powerup;    // 0 = sem objeto   1 = objeto x    2 = objeto y    3 = objeto z ...
    int explosao;   // 0 = sem explosao   1 = explosao central   2 = explosao horizontal   3 = explosao vertical (da bomba)
    int personagem; // -1 = um inimigo   0 = nada   1 = um jogador
}tile;

typedef struct servidorcomunicacao{
    int estado;             // 0 = acabou   1 = jogo a decorrer   2 = kick
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
    int continua;
}infoglobal;
    
void adduser(char* nomefich, char* cmd);

void kickplayer_ativo(char* cmd);

int addnewcliente_ativo(clogin newcli, int nmaxplay);

void set_atributos_newcli();

void re_nascimento(int num_cli);

void set_struct_tocliente(servcom* data, char fpid[15]);

void inicializa_mapa(int n_migalhas, enemy* inimigos, int num_inimigos);

void removecliente_ativo(int razao, int cli);

int updownleftright(int x, int y, int x_atual, int y_atual);                    // 0 = tudo bem   1 = morreu   2 = inimigo matou jogador

void trataevalida_tecla(cmov movcli);

void trataevalida_tecla_inimigo(char tecla, enemy* dados_inimigo);               

void* tratateclado(void* data_trata_cmd);

void* enemy_thread(void *threadarg);

void* bomb_mega_bomb(void *range);

void explode_bomba(bomb_mb bomba, enemy* inimigos, int num_inimigos);

void fazupdate();

int cliente_reconhecido(char* nomefich, clogin teste);

void thread_sigusr2(int sig);

void shutdown_sigusr1(int sig);

#ifdef __cplusplus
}
#endif

#endif /* SERVIDOR_H */
