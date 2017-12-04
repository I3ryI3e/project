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
#define LIN 21
#define COL 31

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
    dcli* aux;
    int fifoaux, kick=2;
    
    token = strtok(cmd, " ");
    if((kickplayer = strtok(NULL, " ")) != NULL){
        for(int i=0;i<info.cli_activos;i++){
            if(strcmp(kickplayer, info.clientes_activos->dados_cli.username)==0){
                if((fifoaux=open(info.clientes_activos->dados_cli.fifopid, O_WRONLY)) < 0){
                    perror("Erro ao abrir o fifo cliente\n");
                    return;
                }
                if(write(fifoaux,&kick,sizeof(kick)) < 0){
                    perror("Erro ao escrever para fifo\n");
                }
                if((aux = realloc(info.clientes_activos, (sizeof(dcli)*(info.cli_activos-1)))) == NULL){
                    perror("Erro na realocacao de memoria\n");
                    return;
                }else{
                    info.clientes_activos=aux;
                    --info.cli_activos;
                    //NAO ESTA ACABADO TOCA A TRABALHAR
                }
            }
        }
    }else{
        perror("Erro: parametros invalidos\n");
    }
}                                 //FALTA MEXER NO MAPA, E PRECISO MUDAR A ESTRUTURA MAPA!!!    E O NUMERO DO KICK

int addnewcliente_ativo(clogin newcli, int nmaxplay){
    
    dcli* aux;
    
    if(info.cli_activos == 0){
        if((info.clientes_activos=malloc(sizeof(dcli))) == NULL){
            perror("Erro na alocacao de memoria\n");
            return 0;
        }else{
            info.cli_activos=1;
            info.clientes_activos[0].dados_cli = newcli;
            set_atributos_newcli(&info.clientes_activos[0].atributos_cli);
            return 1;
        }
    }
    if(nmaxplay == 1)
        return 0;
    if((info.cli_activos+1) == nmaxplay){
        return 0;
    }else{
        if((aux = realloc(info.clientes_activos, (sizeof(dcli)*(info.cli_activos+1)))) == NULL){
            perror("Erro na realocacao de memoria\n");
            return 0;
        }else{
            ++info.cli_activos;
            info.clientes_activos=aux;
            info.clientes_activos[info.cli_activos-1].dados_cli = newcli;
            set_atributos_newcli(&info.clientes_activos[info.cli_activos-1].atributos_cli);
            return 1;
        }
    }
}             //USA OS MUTEXES

void set_atributos_newcli(jogador* atributos){
    
    atributos->x=0;
    atributos->y=0;
    atributos->bomb=3;
    atributos->megabomb=2;
    atributos->nvidas=2;
    atributos->pontuacao=0;
    atributos->items=0;
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
        }
    }
}

void inicializa_mapa(){
    
    srand((unsigned) time(NULL));
    int aux;
    
    for(int i=0;i<LIN;i++){
        for(int j=0;j<COL;j++){
            info.mapa[i][j].explosao=0;
            info.mapa[i][j].powerup=0;
            if((i==0 && (j==0 || j==1 || j==(COL-2) || j==(COL-1))) || (i==1 && (j==0 || j==(COL-1))) ||
                    (i==(LIN-1) && (j==0 || j==1 || j==(COL-2) || j==(COL-1))) || (i==(LIN-2) && (j==0 || j==(COL-1)))){
                info.mapa[i][j].wall=0;
            }else if(i%2 != 0 && j%2 != 0){
                info.mapa[i][j].wall=1;
            }else{
                aux = (rand()%100);
                if(aux < 69)
                    info.mapa[i][j].wall=2;
                else
                    info.mapa[i][j].wall=0;
            }
        }
    }
    //FAZER AS MIGALHAS
    return;
}                                           //FAZER MIGALHAS

//int trataevalida_tecla(char tecla){}

void* tratateclado(void* tratacmd_running){
    
    char* cmd;
    char linha[100], token[100], kickplayer[30];
    int openfifo, encerrar=0;
    
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
            adduser(info.nomefich, linha);
            continue;
        }
        if(!strcmp(cmd, "users")){
            if(info.cli_activos==0)
                printf("Nao existem jogadores ativos\n");
            else
                printf("Jogadores ativos:\n");
            for(int i=0;i<info.cli_activos;i++){
                printf("%d: %s\n", i, info.clientes_activos->dados_cli.username);
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
            pthread_mutex_lock(&lock);
            info.continua=0;
            pthread_mutex_unlock(&lock);
            *((int*)tratacmd_running)=0;
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

    int openfifo, tipomsg, resposta, fifocliente, tratacmd_running=1;
    int nobject, nenemy, nmaxplay;
    clogin login;
    cmov movimento;
    servcom dados_jogo;
    pthread_t thread_tratateclado;
    
    if(argc != 2){
        printf("Sintaxe: %s nome_do_ficheiro\n", argv[0]);
        return (EXIT_FAILURE);
    }
    if(getenv("NOBJECT") == NULL){
        srand((unsigned) time(NULL));
        nobject=(rand()%25)+5;
    }else{
        nobject=atoi(getenv("NOBJECT"));
    }
    if(getenv("NENEMY") == NULL){
        srand((unsigned) time(NULL));
        nenemy=(rand()%5)+1;
    }else{
        nenemy=atoi(getenv("NENEMY"));
    }
    if(getenv("NMAXPLAY") == NULL){
        srand((unsigned) time(NULL));
        nmaxplay=(rand()%20)+1;
    }else{
        nmaxplay=atoi(getenv("NMAXPLAY"));
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
    
    strcpy(info.nomefich, argv[1]);                                           
    info.clientes_activos=NULL;
    info.continua=1;
    inicializa_mapa();
    
    if(pthread_create(&thread_tratateclado, NULL, tratateclado, (void*)&tratacmd_running) != 0){
        perror("Erro ao criar a thread\n");
        return(EXIT_FAILURE);
    }
    
    while(info.continua){
        if(read(openfifo,&tipomsg,sizeof(tipomsg)) < 0){
            perror("Erro na leitura do fifo\n");
            break;
        }
        if(tipomsg==1){
            if(read(openfifo,&login,sizeof(login)) < 0){
                perror("Erro na leitura do fifo\n");
                break;
            }
            if(resposta=cliente_reconhecido(info.nomefich, login))              //APAGAR O IF ELSE
                printf("cliente bom\n");
            else
                printf("cliente mau\n");
   
            if((fifocliente=open(login.fifopid, O_WRONLY)) < 0){
                perror("Erro ao abrir o fifo cliente\n");
            }
            if(resposta){
                if(addnewcliente_ativo(login, nmaxplay)){
                    if(write(fifocliente,&resposta,sizeof(resposta)) < 0){      //RESPOSTA = 1 => LOGIN ACEITE E ADICIONADO
                        perror("Erro a escrever para cliente\n");
                        break;
                    }
                }else{
                    resposta=-1;
                    if(write(fifocliente,&resposta,sizeof(resposta)) < 0){      //RESPOSTA = -1 => LOGIN ACEITE MAS NAO FOI ADICIONADO
                        perror("Erro a escrever para cliente\n");
                        close(fifocliente);
                        break;
                    }
                }
                set_struct_tocliente(&dados_jogo, login.fifopid);
                if(write(fifocliente,&dados_jogo,sizeof(dados_jogo)) < 0){      //ENVIO DA ESTRUTURA DE DADOS PARA O CLIENTE
                    perror("Erro a escrever para cliente\n");
                }
            }else{
                if(write(fifocliente,&resposta,sizeof(resposta)) < 0){          //RESPOSTA = 0 => LOGIN REJEITADO
                    perror("Erro a escrever para cliente\n");
                }
            }
            close(fifocliente);
            continue;
        }
        if(tipomsg == 2){
            if(read(openfifo,&movimento,sizeof(movimento)) < 0){
                perror("Erro na leitura do fifo\n");
                break;
            }
            //trataevalida_tecla(movimento.tecla);
        }
    }
    if(tratacmd_running)
        pthread_kill(thread_tratateclado, SIGUSR2);

    close(openfifo);
    unlink("/tmp/fifoserv");
    pthread_join(thread_tratateclado, NULL);
    pthread_mutex_destroy(&lock);
    free(info.clientes_activos);
    return(EXIT_SUCCESS);
}