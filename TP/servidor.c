#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "servidor.h"

infoglobal info;
pthread_mutex_t lock;

void adduser(char* nomefich, char* cmd){  
    FILE *f;
    clogin new, aux;
    int flag=0;
    char* token;
    char* pass;
    char* user;
    
    token = strtok(cmd, " ");
    if((user = strtok(NULL, " ")) == NULL){
        flag=1;
    }
    if((pass = strtok(NULL, " ")) == NULL){
        flag=1;
    }
    if((token = strtok(NULL, " ")) != NULL){
        flag=1;
    }
    if(flag){
        perror("Erro: parametros invalidos\n");
        return;
    }
    strncpy(new.username, user, 29);
    strncpy(new.password, pass, 29);
    
    if((f = fopen(nomefich, "a+")) == NULL){
        perror("Erro ao abrir o ficheiro\n");
        return;
    }
    while(fscanf(f, " %29s", aux.username) != EOF){
        fscanf(f, " %29s", aux.password);
        if(strcmp(new.username, aux.username) == 0){
            perror("Erro: nome do utilizador ja existente\n");
            fclose(f);
            return;
        }
    }
    fprintf(f, "%s\n%s\n", new.username, new.password);
    
    fclose(f);
    return;
}

void kickplayer_ativo(char* cmd){  
    char* token;
    char* kickplayer;
    
    token = strtok(cmd, " ");
    if((kickplayer = strtok(NULL, " ")) != NULL){
        for(int i=0;i<info.cli_activos;i++){
            if(strcmp(kickplayer, info.clientes_activos[i].dados_cli.username)==0){
                removecliente_ativo(2, i);
                i = info.cli_activos;
            }
        }
    }else{
        perror("Erro: parametros invalidos\n");
    }
}

int addnewcliente_ativo(clogin newcli, int nmaxplay){ 
    dcli* aux;
    if(info.cli_activos == 0){
        if((info.clientes_activos=malloc(sizeof(dcli))) == NULL){
            perror("Erro na alocacao de memoria\n");
            return 0;
        }else{
            info.clientes_activos[info.cli_activos].dados_cli = newcli;
            set_atributos_newcli();
            info.cli_activos=1;
            return 1;
        }
    }
    //if(nmaxplay == 1)
    //    return 0;
    if((info.cli_activos+1) == nmaxplay || nmaxplay == 1){
        return 0;
    }else{
        if((aux = realloc(info.clientes_activos, (sizeof(dcli)*(info.cli_activos+1)))) == NULL){
            perror("Erro na realocacao de memoria\n");
            return 0;
        }else{
            info.clientes_activos=aux;
            info.clientes_activos[info.cli_activos].dados_cli = newcli;
            set_atributos_newcli();
            ++info.cli_activos;
            return 1;
        }
    }
}             //USA OS MUTEXES

void set_atributos_newcli(){  
    info.clientes_activos[info.cli_activos].atributos_cli.bomb=3;
    info.clientes_activos[info.cli_activos].atributos_cli.megabomb=2;
    info.clientes_activos[info.cli_activos].atributos_cli.nvidas=3;
    info.clientes_activos[info.cli_activos].atributos_cli.pontuacao=0;
    info.clientes_activos[info.cli_activos].atributos_cli.items=0;
    re_nascimento(info.cli_activos);
}

void re_nascimento(int num_cli){
    int aux = rand()%4;
    if(aux == 0){
        if(info.mapa[0][0].personagem == 0){
            info.clientes_activos[num_cli].atributos_cli.x=0;
            info.clientes_activos[num_cli].atributos_cli.y=0;
            info.mapa[0][0].personagem = 1;
            return;
        }
    }else{
        if(aux == 1){
            if(info.mapa[0][COL-1].personagem == 0){
                info.clientes_activos[num_cli].atributos_cli.x=0;
                info.clientes_activos[num_cli].atributos_cli.y=(COL-1);
                info.mapa[0][COL-1].personagem = 1;
                return;
            }
        }else{
            if(aux == 2){
                if(info.mapa[LIN-1][0].personagem == 0){
                    info.clientes_activos[num_cli].atributos_cli.x=(LIN-1);
                    info.clientes_activos[num_cli].atributos_cli.y=0;
                    info.mapa[LIN-1][0].personagem = 1;
                    return;
                }
            }else{
                if(info.mapa[LIN-1][COL-1].personagem == 0){
                    info.clientes_activos[num_cli].atributos_cli.x=(LIN-1);
                    info.clientes_activos[num_cli].atributos_cli.y=(COL-1);
                    info.mapa[LIN-1][COL-1].personagem = 1;
                    return;
                }
            }
        }
    }
    for(int i=0;i<LIN;i++){
        for(int j=0;j<COL;j++){
            if(info.mapa[i][j].personagem == 0 && info.mapa[i][j].wall == 0){
                info.clientes_activos[num_cli].atributos_cli.x=i;
                info.clientes_activos[num_cli].atributos_cli.y=j;
                info.mapa[i][j].personagem = 1;
                i = LIN;
                j = COL;
                return;
            }
        }
    }
}

void set_struct_tocliente(servcom* data, char fpid[15]){
    for(int i=0;i<info.cli_activos;i++){
        if((strcmp(fpid, info.clientes_activos[i].dados_cli.fifopid) == 0)){
            data->player.x = info.clientes_activos[i].atributos_cli.x;
            data->player.y = info.clientes_activos[i].atributos_cli.y;
            data->player.bomb = info.clientes_activos[i].atributos_cli.bomb;
            data->player.megabomb = info.clientes_activos[i].atributos_cli.megabomb;
            data->player.nvidas = info.clientes_activos[i].atributos_cli.nvidas;
            data->player.pontuacao = info.clientes_activos[i].atributos_cli.pontuacao;
            data->player.items = info.clientes_activos[i].atributos_cli.items;
            i=info.cli_activos;
        }
    }
    for(int i=0;i<LIN;i++){
        for(int j=0;j<COL;j++){
            data->mapa[i][j].wall = info.mapa[i][j].wall;
            data->mapa[i][j].powerup = info.mapa[i][j].powerup;
            data->mapa[i][j].explosao = info.mapa[i][j].explosao;
            data->mapa[i][j].personagem = info.mapa[i][j].personagem;
        }
    }
}

void inicializa_mapa(int n_migalhas, enemy* inimigos, int num_inimigos){
    int aux, mig=3, i, j, k, mig_aux = n_migalhas/2;
    
    for(i=0;i<LIN;i++){
        for(j=0;j<COL;j++){
            info.mapa[i][j].explosao = 0;
            info.mapa[i][j].powerup = 0;
            info.mapa[i][j].personagem = 0;
            if((i==0 && (j==0 || j==1 || j==(COL-2) || j==(COL-1))) || (i==1 && (j==0 || j==(COL-1))) ||
                    (i==(LIN-1) && (j==0 || j==1 || j==(COL-2) || j==(COL-1))) || (i==(LIN-2) && (j==0 || j==(COL-1)))){
                info.mapa[i][j].wall = 0;
            }else{
                if(i%2 != 0 && j%2 != 0){
                    info.mapa[i][j].wall = 1;
                }else{
                    aux = (rand()%100);
                    if(aux < 69){
                        info.mapa[i][j].wall = 2;
                    }else{
                        if(mig==1 && mig_aux > 0){
                            info.mapa[i][j].wall = 3;
                            --mig_aux;
                            mig = (rand()%10)+1;
                        }else{
                            info.mapa[i][j].wall = 0;
                            --mig;
                        }
                    }
                }
            }
        }
    }
    mig=3;
    mig_aux = (n_migalhas/2) + (n_migalhas%2);
    for(i=LIN-1;i>=0;i--){
        for(j=COL-1;j>=0;j--){
            if(info.mapa[i][j].wall == 0){
                if(mig == 1 && mig_aux > 0){
                    info.mapa[i][j].wall = 3;
                    --mig_aux;
                    mig = (rand()%10)+1;
                }else{
                    --mig;
                }
            }
        }
    }
    mig=1;
    num_inimigos = num_inimigos-1;
    for(k=0;k<12;k++){
        for(i=-1-k;i<2+k;i++){
            for(j=-1-k;j<2+k;j++){
                if(num_inimigos >= 0){
                    if(info.mapa[LIN/2+i][COL/2+j].wall == 0 && mig == 1){
                        info.mapa[LIN/2+i][COL/2+j].personagem = -1;
                        inimigos[num_inimigos].x = LIN/2+i;
                        inimigos[num_inimigos].y = COL/2+i;
                        --num_inimigos;
                        mig = (rand()%5)+1;
                    }else{
                        --mig;
                    }
                }else{
                    return;
                }
            }
        }
    }
    return;
}//PODEMOS ALTERAR A DIFICULDADE AQUI NO RAND

void removecliente_ativo(int razao, int cli){
    dcli* aux;
    servcom kick;
    int fifoaux;
    
    if(cli != (info.cli_activos-1))       //SE NAO E O ULTIMO
        info.clientes_activos[cli] = info.clientes_activos[info.cli_activos-1];   //PEGA-SE NO ULTIMO E METE-SE NO LUGAR ATUAL
    else if(info.cli_activos == 1){     //SE SO EXISTIR UM
        info.cli_activos=0;
        free(info.clientes_activos);
        info.clientes_activos=NULL;
        return;
    }
    if((aux = realloc(info.clientes_activos, (sizeof(dcli)*(info.cli_activos-1)))) == NULL){
        perror("Erro na realocacao de memoria\n");
        return;
    }else{
        info.clientes_activos=aux;
        --info.cli_activos;
        if(razao == 2)
            info.mapa[info.clientes_activos[cli].atributos_cli.x][info.clientes_activos[cli].atributos_cli.y].personagem = 0;
        kick.estado = razao;
    }
    if((fifoaux=open(info.clientes_activos[cli].dados_cli.fifopid, O_WRONLY)) < 0){
        perror("Erro ao abrir o fifo cliente\n");
        return;
    }
    if(write(fifoaux,&kick,sizeof(kick)) < 0){
        perror("Erro ao escrever para fifo\n");
    }
    close(fifoaux);
}

int updownleftrigth(int x, int y, int x_atual, int y_atual){
    if(info.mapa[x_atual+x][y_atual+y].wall == 1 || info.mapa[x_atual+x][y_atual+y].wall == 2)
            return 0;
    if(info.mapa[x_atual+x][y_atual+y].explosao > 0){
        if(info.mapa[x_atual][y_atual].personagem == -1){
            info.mapa[x_atual][y_atual].personagem = 0;
            return 1;   //MORRE INIMIGO
        }else{
            info.mapa[x_atual][y_atual].personagem = 0;
            return 1;   //MORRE CLIENTE
        }
    }
    if(info.mapa[x_atual][y_atual].personagem == -1){
        if(info.mapa[x_atual+x][y_atual+y].personagem == 1){
            info.mapa[x_atual][y_atual].personagem = 0;
            info.mapa[x_atual+x][y_atual+y].personagem = -1;
            return 2;    //INIMIGO MATA JOGADOR
        }
        if(info.mapa[x_atual+x][y_atual+y].personagem != -1){
            info.mapa[x_atual][y_atual].personagem = 0;
            info.mapa[x_atual+x][y_atual+y].personagem = -1;
        }
        return 0;
    }else{
        if(info.mapa[x_atual+x][y_atual+y].personagem == -1){
            info.mapa[x_atual][y_atual].personagem = 0;
            return 1;   //MORRE CLIENTE
        }
        if(info.mapa[x_atual+x][y_atual+y].personagem == 1)
            return 0;
        info.mapa[x_atual][y_atual].personagem = 0;
        info.mapa[x_atual+x][y_atual+y].personagem = 1;
        //PRECISAMOS DE METER OS ATRIBUTOS DO CLIENTE ATUALIZADOS X/Y <------------------<---------<----------
        
        if(info.mapa[x_atual+x][y_atual+y].wall == 3){  //FALTA MIGALHAS E POWERUPS
            //COMO E PARA ACABAR O MAPA, PRECISAMOS DE UMA VARIAVEL QUE GUARDE OS OBJETOS QUE JA SE APANHOU OU OS QUE FALTAM
        }
    }
    //ACABAR FALTA AS MIGALHAS OS POWERUPS
}      //INCOMPLETO + TA MAL UMA CENA

void trataevalida_tecla(cmov movcli){   
    int aux_cli, morto;
    for(int i=0;i<info.cli_activos;i++){
        if(strcmp(movcli.fifopid, info.clientes_activos[i].dados_cli.fifopid)==0){
            aux_cli = i;
            i = info.cli_activos;
        }
    }
    if(movcli.tecla == 'u'){
        if(info.clientes_activos[aux_cli].atributos_cli.y == 0)
            return;
        morto=updownleftrigth(0, 1, info.clientes_activos[aux_cli].atributos_cli.x, info.clientes_activos[aux_cli].atributos_cli.y);
    }
    if(movcli.tecla == 'd'){
        if(info.clientes_activos[aux_cli].atributos_cli.y == (LIN-1))
            return;
        morto=updownleftrigth(0, -1, info.clientes_activos[aux_cli].atributos_cli.x, info.clientes_activos[aux_cli].atributos_cli.y);
    }
    if(movcli.tecla == 'l'){
        if(info.clientes_activos[aux_cli].atributos_cli.x == 0)
            return;
        morto=updownleftrigth(-1, 0, info.clientes_activos[aux_cli].atributos_cli.x, info.clientes_activos[aux_cli].atributos_cli.y);
    }
    if(movcli.tecla == 'r'){
        if(info.clientes_activos[aux_cli].atributos_cli.x == (COL-1))
            return;
        morto=updownleftrigth(1, 0, info.clientes_activos[aux_cli].atributos_cli.x, info.clientes_activos[aux_cli].atributos_cli.y);
    }
    if(morto){
        if(info.clientes_activos[aux_cli].atributos_cli.nvidas == 1){
            removecliente_ativo(0, aux_cli);
            return;
        }
        --info.clientes_activos[aux_cli].atributos_cli.nvidas;
        re_nascimento(aux_cli);
    }
/*
    if(movcli.tecla == 'b'){   //CRIA THREAD
        info.mapa[info.clientes_activos[aux_cli].atributos_cli.x][info.clientes_activos[aux_cli].atributos_cli.y].wall = 4;
        //INICIALIZAR ESTRUTURA DA BOMBA
        if(pthread_create(&NOME DA THREAD, NULL, bomb_mega_bomb, (void*)&STRUCT BOMB_MB) != 0){
            perror("Erro ao criar a thread - bomba\n");
        }   
    }
    if(movcli.tecla == 'm'){   //CRIA THREAD
        info.mapa[info.clientes_activos[aux_cli].atributos_cli.x][info.clientes_activos[aux_cli].atributos_cli.y].wall = 5;
        //INICIALIZAR ESTRUTURA DA BOMBA
        if(pthread_create(&NOME DA THREAD, NULL, bomb_mega_bomb, (void*)&STRUCT BOMB_MB) != 0){
            perror("Erro ao criar a thread - bomba\n");
        }
    }
*/
}

int trataevalida_tecla_inimigo(char tecla, enemy* dados_inimigo){
    if(tecla == 'u'){
        if(dados_inimigo->y == 0)
            return 0;
        return updownleftrigth(0, 1, dados_inimigo->x, dados_inimigo->y);
    }
    if(tecla == 'd'){
        if(dados_inimigo->y == (LIN-1))
            return 0;
        return updownleftrigth(0, -1, dados_inimigo->x, dados_inimigo->y);
    }
    if(tecla == 'l'){
        if(dados_inimigo->x == 0)
            return 0;
        return updownleftrigth(-1, 0, dados_inimigo->x, dados_inimigo->y);
    }
    if(tecla == 'r'){
        if(dados_inimigo->x == (COL-1))
            return 0;
        return updownleftrigth(1, 0, dados_inimigo->x, dados_inimigo->y);
    }
    return 0;
}

void* tratateclado(void* data_trata_cmd){  
    char* cmd;
    char linha[100], token[100];
    int openfifo, encerrar=0;
    trata_cmd *data;
    data = (trata_cmd*) data_trata_cmd;
    
    if(signal(SIGUSR2, thread_sigusr2) == SIG_ERR){
        perror("Erro no sinal - tratateclado\n");
    }
    
    printf("Linha de comandos do servidor\n");
    
    while(info.continua){
        printf("Comando: ");
        scanf(" %[^\n]99s", linha);

        strncpy(token, linha, 99);
        cmd = strtok(token, " ");

        if(!strcmp(cmd, "add")){
            adduser(data->nomefich, linha);
            continue;
        }
        if(!strcmp(cmd, "users")){
            if(info.cli_activos==0)
                printf("Nao existem jogadores ativos\n");
            else
                printf("Jogadores ativos:\n");
            for(int i=0;i<info.cli_activos;i++){
                printf("%d: %s\n", i, info.clientes_activos[i].dados_cli.username);
            }
            continue;
        }
        if(!strcmp(cmd, "kick")){
            kickplayer_ativo(linha);
            continue;
        }
        if(!strcmp(cmd, "game")){
            printf("Mostra informacao sobre o jogo\n");
            continue;
        }
        if(!strcmp(cmd, "shutdown")){
            if((openfifo = open("/tmp/fifoserv", O_WRONLY)) < 0){
                perror("Erro ao abrir o fifo\n");
            }
            //pthread_mutex_lock(&lock);
            info.continua=0;
            //pthread_mutex_unlock(&lock);
            data->estado_thread=0;
            if(write(openfifo,&encerrar,sizeof(encerrar)) < 0){
                perror("Erro ao escrever para fifo\n");
            }
            close(openfifo);
            pthread_exit(NULL);
        }
        if(!strcmp(cmd, "map")){
            printf("Carrega um labirinto\n");
            continue;
        }
        if(!strcmp(cmd, "help")){
            printf("\nComandos reconhecidos:\n\tadd username password;\n\tusers;\n\tkick username;\n\tgame;\n\tshutdown;\n\tmap nome_do_ficheiro;\n");
        }else{
            printf("\nComando nao reconhecido, escreva 'help' para ver os comandos\n");
        }
    }
    pthread_exit(NULL);
}                         //TIRAR SINAL DAQUI

void* enemy_thread(void *threadarg){
    
    enemy *data;
    data = (enemy*) threadarg;
    int openfifo, aux;
    cmov movimento;
    
    pause();
    
    while(data->vida){
        aux = rand()%4;
        if(aux == 0){
            movimento.tecla = 'u';
        }else
            if(aux == 1)
                movimento.tecla = 'd';
            else
                if(aux == 2)
                    movimento.tecla = 'l';
                else
                    movimento.tecla = 'r';
        if((openfifo = open("/tmp/fifoserv", O_WRONLY)) < 0){
            perror("Erro ao abrir o fifo\n");
        }
        movimento.fifopid[0] = data->id;
        if(write(openfifo,&movimento,sizeof(movimento)) < 0){
            perror("Erro ao escrever para fifo\n");
        }
        close(openfifo);
        sleep(1);
    }
    pthread_exit(NULL);
}

void* bomb_mega_bomb(void* range){
    bomb_com *bomba = (bomb_com*) range;
    int openfifo;
    sleep(2);
    if((openfifo = open("/tmp/fifoserv", O_WRONLY)) < 0){
        perror("Erro ao abrir o fifo\n");
    }
    if(write(openfifo,&bomba,sizeof(bomb_com)) < 0){
        perror("Erro ao escrever para fifo\n");
    }
    sleep(2);
    bomba->bomb_type += 1;
    if(write(openfifo,&bomba,sizeof(bomb_com)) < 0){
        perror("Erro ao escrever para fifo\n");
    }
    close(openfifo);
    //  ACABAR AQUI CENAS <-----------------<-----------------<-----------------
    pthread_exit(NULL);
}

void explode_bomba(bomb_mb bomba, enemy* inimigos, int num_inimigos){
    int i, j, k, l, m, aux = bomba.range-2;
    i=j=k=l=1;
    
    info.mapa[bomba.x][bomba.y].wall = 0;
    info.mapa[bomba.x][bomba.y].explosao = 1;
    while(aux>0){
        if(i<=bomba.range && (bomba.x+i)<=LIN-1){
            if(info.mapa[bomba.x+i][bomba.y].wall == 1){
                i=bomba.range+1;
            }else{
                if(info.mapa[bomba.x+i][bomba.y].wall == 2){
                    info.mapa[bomba.x+i][bomba.y].wall = 0;
                    info.mapa[bomba.x+i][bomba.y].explosao = 2;
                    i=bomba.range+1;
                }else{
                    info.mapa[bomba.x+i][bomba.y].explosao = 2;
                    if(info.mapa[bomba.x+i][bomba.y].personagem == 1){
                        if(updownleftrigth(i, 0, bomba.x, bomba.y) == 1){
                            for(m=0;m<info.cli_activos;m++){
                                if(info.clientes_activos[m].atributos_cli.x == (bomba.x+i) && info.clientes_activos[m].atributos_cli.y == (bomba.y)){
                                    if(info.clientes_activos[m].atributos_cli.nvidas == 1){
                                        removecliente_ativo(0, m);
                                    }else{
                                        --info.clientes_activos[m].atributos_cli.nvidas;
                                        info.mapa[info.clientes_activos[m].atributos_cli.x][info.clientes_activos[m].atributos_cli.y].personagem = 0;
                                        re_nascimento(m);
                                    }
                                }
                            }
                        }
                    }
                    if(info.mapa[bomba.x+i][bomba.y].personagem == -1){
                        if(updownleftrigth(i, 0, bomba.x, bomba.y) == 1){
                            for(m=0;m<num_inimigos;m++){
                                if((inimigos[m].x == (bomba.x+i)) && (inimigos[m].y == (bomba.y)))
                                    inimigos[m].vida = 0;
                            }
                        }
                    }
                    ++i;
                }
            }
        }
        if(j<=bomba.range && (bomba.x-j)>=0){
            if(info.mapa[bomba.x-j][bomba.y].wall == 1){
                j=bomba.range+1;
            }else{
                if(info.mapa[bomba.x-j][bomba.y].wall == 2){
                    info.mapa[bomba.x-j][bomba.y].wall = 0;
                    info.mapa[bomba.x-j][bomba.y].explosao = 2;
                    j=bomba.range+1;
                }else{
                    info.mapa[bomba.x-j][bomba.y].explosao = 2;
                    if(info.mapa[bomba.x-j][bomba.y].personagem == 1){
                        if(updownleftrigth(-j, 0, bomba.x, bomba.y) == 1){
                            for(m=0;m<info.cli_activos;m++){
                                if(info.clientes_activos[m].atributos_cli.x == (bomba.x-j) && info.clientes_activos[m].atributos_cli.y == (bomba.y)){
                                    if(info.clientes_activos[m].atributos_cli.nvidas == 1){
                                        removecliente_ativo(0, m);
                                    }else{
                                        --info.clientes_activos[m].atributos_cli.nvidas;
                                        info.mapa[info.clientes_activos[m].atributos_cli.x][info.clientes_activos[m].atributos_cli.y].personagem = 0;
                                        re_nascimento(m);
                                    }
                                }
                            }
                        }
                    }
                    if(info.mapa[bomba.x-j][bomba.y].personagem == -1){
                        if(updownleftrigth(-j, 0, bomba.x, bomba.y) == 1){
                            for(m=0;m<num_inimigos;m++){
                                if((inimigos[m].x == (bomba.x-j)) && (inimigos[m].y == (bomba.y)))
                                    inimigos[m].vida = 0;
                            }
                        }
                    }
                    ++j;
                }
            }
        }
        if(k<=bomba.range && (bomba.y+k)<=COL-1){
            if(info.mapa[bomba.x][bomba.y+k].wall == 1){
                k=bomba.range+1;
            }else{
                if(info.mapa[bomba.x][bomba.y+k].wall == 2){
                    info.mapa[bomba.x][bomba.y+k].wall = 0;
                    info.mapa[bomba.x][bomba.y+k].explosao = 2;
                    k=bomba.range+1;
                }else{
                    info.mapa[bomba.x][bomba.y+k].explosao = 2;
                    if(info.mapa[bomba.x][bomba.y+k].personagem == 1){
                        if(updownleftrigth(0, k, bomba.x, bomba.y) == 1){
                            for(m=0;m<info.cli_activos;m++){
                                if(info.clientes_activos[m].atributos_cli.x == (bomba.x) && info.clientes_activos[m].atributos_cli.y == (bomba.y+k)){
                                    if(info.clientes_activos[m].atributos_cli.nvidas == 1){
                                        removecliente_ativo(0, m);
                                    }else{
                                        --info.clientes_activos[m].atributos_cli.nvidas;
                                        info.mapa[info.clientes_activos[m].atributos_cli.x][info.clientes_activos[m].atributos_cli.y].personagem = 0;
                                        re_nascimento(m);
                                    }
                                }
                            }
                        }
                    }
                    if(info.mapa[bomba.x][bomba.y+k].personagem == -1){
                        if(updownleftrigth(0, k, bomba.x, bomba.y) == 1){
                            for(m=0;m<num_inimigos;m++){
                                if((inimigos[m].x == (bomba.x)) && (inimigos[m].y == (bomba.y+k)))
                                    inimigos[m].vida = 0;
                            }
                        }
                    }
                    ++k;
                }
            }
        }
        if(l<=bomba.range && (bomba.y-l)>=0){
            if(info.mapa[bomba.x][bomba.y-l].wall == 1){
                l=bomba.range+1;
            }else{
                if(info.mapa[bomba.x][bomba.y-l].wall == 2){
                    info.mapa[bomba.x][bomba.y-l].wall = 0;
                    info.mapa[bomba.x][bomba.y-l].explosao = 2;
                    l=bomba.range+1;
                }else{
                    info.mapa[bomba.x][bomba.y-l].explosao = 2;
                    if(info.mapa[bomba.x][bomba.y-l].personagem == 1){
                        if(updownleftrigth(0, -l, bomba.x, bomba.y) == 1){
                            for(m=0;m<info.cli_activos;m++){
                                if(info.clientes_activos[m].atributos_cli.x == (bomba.x) && info.clientes_activos[m].atributos_cli.y == (bomba.y-l)){
                                    if(info.clientes_activos[m].atributos_cli.nvidas == 1){
                                        removecliente_ativo(0, m);
                                    }else{
                                        --info.clientes_activos[m].atributos_cli.nvidas;
                                        info.mapa[info.clientes_activos[m].atributos_cli.x][info.clientes_activos[m].atributos_cli.y].personagem = 0;
                                        re_nascimento(m);
                                    }
                                }
                            }
                        }
                    }
                    if(info.mapa[bomba.x][bomba.y-l].personagem == -1){
                        if(updownleftrigth(0, -l, bomba.x, bomba.y) == 1){
                            for(m=0;m<num_inimigos;m++){
                                if((inimigos[m].x == (bomba.x)) && (inimigos[m].y == (bomba.y-l)))
                                    inimigos[m].vida = 0;
                            }
                        }
                    }
                    ++l;
                }
            }
        }
        --aux;
    }
}

void fazupdate(){
    int fd;
    servcom comunicacao;
    for(int i=0;i<info.cli_activos;i++){
        comunicacao.estado=1;
        set_struct_tocliente(&comunicacao,info.clientes_activos[i].dados_cli.fifopid);
        if((fd=open(info.clientes_activos[i].dados_cli.fifopid, O_WRONLY)) <0)
            perror("Erro ao abrir fifo\n");
        if(write(fd,&comunicacao,sizeof(servcom)) < 0)
            perror("Erro a escrever no fifo\n");
        close(fd);
    }
}

void thread_sigusr2(int sig){
    pthread_exit(NULL); 
}

void shutdown_sigusr1(int sig){  
    int openfifo, encerrar=0;
    if((openfifo = open("/tmp/fifoserv", O_WRONLY)) < 0){
            perror("Erro ao abrir o fifo\n");
        }
        pthread_mutex_lock(&lock);
        info.continua=0;
        pthread_mutex_unlock(&lock);
        if(write(openfifo,&encerrar,sizeof(encerrar)) < 0){
            perror("Erro ao escrever para fifo\n");
        }
        close(openfifo);
        return;
}

int cliente_reconhecido(char* nomefich, clogin teste){ 
    FILE *f;
    clogin aux;
    
    if((f = fopen(nomefich, "rt")) == NULL){
        perror("Erro ao abrir o ficheiro\n");
        return 0;
    }
    while(fscanf(f, " %29s", aux.username) != EOF){
        fscanf(f, " %29s", aux.password);
        if(strcmp(teste.username, aux.username) == 0){
            if(strcmp(teste.password, aux.password) == 0){
                fclose(f);
                return 1;
            }
        }
    }
    fclose(f);
    return 0;
}

int main(int argc, char** argv){
    int openfifo, tipomsg, resposta, fifocliente, i;
    int nobject, nenemy, nmaxplay;
    trata_cmd data_trata_cmd;
    bomb_mb bomb_msg;
    clogin login;
    cmov movimento;
    servcom dados_jogo;
    pthread_t thread_tratateclado;
    srand((unsigned) time(NULL));
    
    if(argc != 2){
        printf("Sintaxe: %s nome_do_ficheiro\n", argv[0]);
        return (EXIT_FAILURE);
    }
    if(getenv("NOBJECT") == NULL)
        nobject=(rand()%25)+5;
    else
        nobject=atoi(getenv("NOBJECT"));
    if(getenv("NMAXPLAY") == NULL)
        nmaxplay=(rand()%20)+1;
    else
        nmaxplay=atoi(getenv("NMAXPLAY"));
    if(getenv("NENEMY") == NULL)
        nenemy=(rand()%5)+1;
    else
        nenemy=atoi(getenv("NENEMY"));
    
    pthread_t enemies[nenemy];
    enemy *inimigos_ativos;
    int num_enemies = nenemy;
    if((inimigos_ativos=malloc(sizeof(enemy) * (num_enemies))) == NULL){
        perror("Erro na alocacao de memoria\n");
        return (EXIT_FAILURE);
    } 
    if(signal(SIGUSR1, shutdown_sigusr1) == SIG_ERR){
        perror("Erro no sinal\n");
        return(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("Inicilizacao do mutex falhou\n");
        return (EXIT_FAILURE);
    }
    if(mkfifo("/tmp/fifoserv", S_IWUSR | S_IRUSR) != 0)
        return(EXIT_FAILURE);
    
    if((openfifo = open("/tmp/fifoserv", O_RDWR)) < 0){
        perror("Erro ao abrir o fifo\n");
        return(EXIT_FAILURE);
    }
                                             
    info.clientes_activos=NULL;
    info.cli_activos=0;
    info.continua=1;
    inicializa_mapa(nobject, inimigos_ativos, num_enemies);
    
    data_trata_cmd.estado_thread = 1;
    strcpy(data_trata_cmd.nomefich, argv[1]); 
    if(pthread_create(&thread_tratateclado, NULL, tratateclado, (void*)&data_trata_cmd) != 0){
        perror("Erro ao criar a thread\n");
        return(EXIT_FAILURE);
    }
    for(i=0;i<num_enemies;i++){
        inimigos_ativos[i].vida = 1;
        inimigos_ativos[i].id = i;
        if(pthread_create(&enemies[i], NULL, enemy_thread, (void*)&inimigos_ativos[i]) != 0){
            perror("Erro ao criar a thread\n");
            return(EXIT_FAILURE);
        }
    }
    
    while(info.continua){
        if(read(openfifo,&tipomsg,sizeof(int)) < 0){
            perror("Erro na leitura do fifo\n");
            break;
        }
        if(tipomsg==1){
            if(read(openfifo,&login,sizeof(clogin)) < 0){
                perror("Erro na leitura do fifo\n");
                break;
            }
            if(resposta=cliente_reconhecido(argv[1], login))                    //APAGAR O IF ELSE
                printf("cliente bom\n");
            else
                printf("cliente mau\n");
   
            if((fifocliente=open(login.fifopid, O_WRONLY)) < 0){
                perror("Erro ao abrir o fifo cliente\n");
            }
            if(resposta){
                if(addnewcliente_ativo(login, nmaxplay)){
                    if(write(fifocliente,&resposta,sizeof(int)) < 0){      //RESPOSTA = 1 => LOGIN ACEITE E ADICIONADO
                        perror("Erro a escrever para cliente\n");
                        break;
                    }
                    dados_jogo.estado=1;
                    set_struct_tocliente(&dados_jogo, login.fifopid);
                    if(write(fifocliente,&dados_jogo,sizeof(servcom)) < 0){  //ENVIO DA ESTRUTURA DE DADOS PARA O CLIENTE
                        perror("Erro a escrever para cliente\n");
                    }
                }else{
                    resposta=-1;
                    if(write(fifocliente,&resposta,sizeof(int)) < 0){      //RESPOSTA = -1 => LOGIN ACEITE MAS NAO FOI ADICIONADO
                        perror("Erro a escrever para cliente\n");
                        close(fifocliente);
                        break;
                    }
                }
            }else{
                if(write(fifocliente,&resposta,sizeof(int)) < 0){          //RESPOSTA = 0 => LOGIN REJEITADO
                    perror("Erro a escrever para cliente\n");
                }
            }
            close(fifocliente);
            continue;
        }
        if(tipomsg == 2){
            if(read(openfifo,&movimento,sizeof(cmov)) < 0){
                perror("Erro na leitura do fifo\n");
                break;
            }
            if(movimento.fifopid[0] == '/')
                trataevalida_tecla(movimento);
            else
                for(i=0;i<num_enemies;i++)
                    if(movimento.fifopid[0] == inimigos_ativos[i].id && inimigos_ativos[i].vida == 1)
                        if(trataevalida_tecla_inimigo(movimento.tecla, &inimigos_ativos[i]) != 0)
                            if(trataevalida_tecla_inimigo(movimento.tecla, &inimigos_ativos[i]) == 1)
                                 inimigos_ativos[i].vida = 0;
                            else
                                for(int j=0;j<info.cli_activos;j++){
                                    if(info.clientes_activos[j].atributos_cli.x == (inimigos_ativos[i].x) && info.clientes_activos[j].atributos_cli.y == (inimigos_ativos[i].y)){
                                        if(info.clientes_activos[j].atributos_cli.nvidas == 1){
                                            removecliente_ativo(0, j);
                                        }else{
                                            --info.clientes_activos[j].atributos_cli.nvidas;
                                            re_nascimento(j);
                                        }
                                    }
                                }
            fazupdate(); 
            continue;
        }
        if(tipomsg == 3){
            
        }
        if(tipomsg == 4){
            if(read(openfifo,&bomb_msg,sizeof(bomb_mb)) < 0){
                perror("Erro na leitura do fifo\n");
                break;
            }
            explode_bomba(bomb_msg, inimigos_ativos, num_enemies);
            fazupdate();
            continue;
        }
    }
    if(data_trata_cmd.estado_thread)
        pthread_kill(thread_tratateclado, SIGUSR2);
    
    for(i=0;i<info.cli_activos;i++){
        if((fifocliente=open(info.clientes_activos[i].dados_cli.fifopid, O_WRONLY)) < 0){
            perror("Erro ao abrir o fifo cliente\n");
        }
        dados_jogo.estado = 0;
        if(write(fifocliente,&dados_jogo,sizeof(dados_jogo)) < 0){
            perror("Erro a escrever para cliente\n");
        }
        close(fifocliente);
    }
    close(openfifo);
    unlink("/tmp/fifoserv");
    pthread_join(thread_tratateclado, NULL);
    for(int i=0;i<num_enemies;i++){
        pthread_join(enemies[i], NULL);
    }
    pthread_mutex_destroy(&lock);
    free(inimigos_ativos);
    free(info.clientes_activos);
    return(EXIT_SUCCESS);
}