#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "servidor.h"
#include "dados.h"

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
        perror("Erro na validacao dos parametros\n");
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

void inicializa_com(servcom* data){
    data->player.x=0;
    data->player.y=0;
    data->player.bomb=3;
    data->player.megabomb=2;
    data->player.nvidas= 2;
    data->player.pontuacao=0;
    data->player.items=0;
}

void* tratateclado(void* lixo){
    
    char* cmd;
    char linha[100], token[100];
    int openfifo;
    int encerrar=0;
    
    while(info.continua){
        scanf(" %[^\n]99s", linha);

        strncpy(token, linha, 99);
        cmd = strtok(token, " ");

        if(!strcmp(cmd, "add")){
            adduser(info.nomefich, linha);
        }else{
            if(!strcmp(cmd, "users")){
                printf("Mostra users\n");
            }else{
                if(!strcmp(cmd, "kick")){
                    printf("Termina a ligacao do jogador\n");
                }else{
                    if(!strcmp(cmd, "game")){
                        printf("Mostra informacao sobre o jogo\n");
                    }else{
                        if(!strcmp(cmd, "shutdown")){
                            if((openfifo = open("/tmp/fifoserv", O_WRONLY)) < 0){
                                perror("Erro ao abrir o fifo\n");
                             }
                            pthread_mutex_lock(&lock);
                            info.continua=0;
                            pthread_mutex_unlock(&lock);
                            if(write(openfifo,&encerrar,sizeof(encerrar)) < 0){
                                perror("Erro ao escrever para fifo\n");
                            }
                            pthread_exit(NULL);
                        }else{
                            if(!strcmp(cmd, "map")){
                                printf("Carrega um labirinto\n");
                            }else{
                                if(!strcmp(cmd, "help")){
                                    printf("\nComandos reconhecidos:\n\tadd username password;\n\tusers;\n\tkick username;\n\tgame;\n\tshutdown;\n\tmap nome_do_ficheiro;\n");
                                }else{
                                    printf("\nComando nao reconhecido, escreva 'help' para ver os comandos\n");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    pthread_exit(NULL);
}

void shutdown(int sig){
    
    close(3);                                                                   //NAO PODE SER 3
    unlink("/tmp/fifoserv");
    exit(EXIT_SUCCESS);
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

    int openfifo,tipomsg, resposta,fifocliente;
    clogin login;
    cmov movimento;
    servcom dados_jogo;
    pthread_t thread;
    void *status;
    
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Inicilizacao do mutex falhou\n");
        return (EXIT_FAILURE);
    }
    if(signal(SIGUSR1, shutdown) == SIG_ERR){
        perror("Erro no sinal\n");
        return(EXIT_FAILURE);
    }
    
    if(argc != 2){                                                              //ARGC TEM QUE SER 2 PK RECEBE O NOME DO FICHEIRO
        printf("Sintaxe: %s nome_do_ficheiro\n", argv[0]);
        return (EXIT_FAILURE);
    }
    strcpy(info.nomefich, argv[1]);                                           
    info.clientes_activos=NULL;
    info.continua=1;
    
    if(mkfifo("/tmp/fifoserv", S_IWUSR | S_IRUSR) != 0)
        return(EXIT_FAILURE);
    
    if((openfifo = open("/tmp/fifoserv", O_RDWR)) < 0){
        perror("Erro ao abrir o fifo\n");
        return(EXIT_FAILURE);
    }
    
    printf("Mensagem de boas vindas..\n");                                      //MUDAR ISTO
    
    if(pthread_create(&thread, NULL, tratateclado, (void*)NULL) != 0){
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
        
            if(resposta=cliente_reconhecido("data.txt", login))
                printf("Jogador reconhecido\n");
            else
                printf("Jogador falso\n");
            if((fifocliente= open(login.fifopid, O_WRONLY)) < 0){
                perror("Erro ao abrir o fifo cliente\n");
            }
            if(write(fifocliente,&resposta,sizeof(resposta)) < 0){
                perror("Erro a escrever para cliente\n");
            }
            if(resposta){
                inicializa_com(&dados_jogo);
                if(write(fifocliente,&dados_jogo,sizeof(dados_jogo)) < 0){
                    perror("Erro a escrever para cliente\n");
                }
            }
            close(fifocliente);
            
        }else if(tipomsg == 2){
            if(read(openfifo,&movimento,sizeof(movimento)) < 0){
                perror("Erro na leitura do fifo\n");
                break;
            }
        }
    }

    pthread_join(thread, &status);
    shutdown(0);
}