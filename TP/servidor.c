/* 
 * File:   servidor.c
 * Author: fclfi
 *
 * Created on 25 de Outubro de 2017, 18:46
 */

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

void adduser(char* nomefich, char* cmd){
    
    FILE *f;
    clicom new, aux;
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
    

/*
    if((file = open(nomefich, O_RDONLY)) < 0){
        perror("Erro ao abrir o ficheiro\n");
        return;       
    }
    while(read(file, &aux, sizeof(aux)) != 0){
        if(strcmp(new.username, aux.username) == 0){
            perror("Erro: nome do utilizador ja existente\n");
            close(file);
            return;
        }
    }
    close(file);
    if((file = open(nomefich, O_WRONLY | O_APPEND)) < 0){
        perror("Erro ao abrir o ficheiro\n");
        return;       
    }
    if(write(file, &new, sizeof(aux)) != sizeof(aux)){
        close(file);
        perror("Erro na escrita do novo utilizador\n");
        return;
    }
    close(file);
*/
    
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

void* tratateclado(void* shutd){
    
    char* cmd;
    char linha[100], token[100];
    int *shut = (int*)shutd;
    
    while(1){
        scanf(" %[^\n]99s", linha);

        strncpy(token, linha, 99);
        cmd = strtok(token, " ");

        if(!strcmp(cmd, "add")){
            adduser("data.txt", linha);
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
                            printf("Vai sair\n");
                            *shut = 0;
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

int checkcliente(char* nomefich, clicom teste){
    
    int file;
    clicom aux;
    
    if((file = open(nomefich, O_RDONLY)) < 0){
        perror("Erro ao abrir o ficheiro\n");
        return 1;       
    }
    while(read(file, &aux.username, sizeof(aux.username)) != 0){
        read(file, &aux.password, sizeof(aux.password));
        if(strcmp(teste.username, aux.username) == 0){
            if(strcmp(teste.password, aux.password) == 0)
                return 0;
        }
    }
    close(file);
    return 1;
}

int main(int argc, char** argv){

    FILE *f;
    int openfile, checkcmd = 1;
    clicom teste;
    pthread_t thread;
    void *status;
    
    if(signal(SIGUSR1, shutdown) == SIG_ERR){
        perror("Erro no sinal\n");
        return(EXIT_FAILURE);
    }
    
    if(argc != 1){                                                              //ARGC TEM QUE SER 2 PK RECEBE O NOME DO FICHEIRO
        printf("Sintaxe: %s nome_do_ficheiro\n", argv[0]);
        return (EXIT_FAILURE);
    }
    if((f = fopen("data.txt", "rt")) == NULL){
        perror("Erro: Ficheiro nao encontrado\n");
        return (EXIT_FAILURE);
    }
    fclose(f);
    
    if(mkfifo("/tmp/fifoserv", S_IWUSR | S_IRUSR) != 0)
        return(EXIT_FAILURE);
    
    if((openfile = open("/tmp/fifoserv", O_RDWR)) < 0){
        perror("Erro ao abrir o ficheiro\n");
        return(EXIT_FAILURE);
    }
    
    printf("Mensagem de boas vindas..\n");                                      //MUDAR ISTO
    
    if(pthread_create(&thread, NULL, tratateclado, (void*)&checkcmd) != 0){
        perror("Erro ao criar a thread\n");
        return(EXIT_FAILURE);
    }
    
    while(checkcmd){
        if(read(openfile,&teste,sizeof(teste)) < 0){
            perror("Erro na leitura do fifo\n");
            break;
        }
        if(checkcliente("data.txt", teste))
            printf("Jogador falso\n");
        else
            printf("Jogador reconhecido\n");
    }

    pthread_join(thread, &status);
    shutdown(0);
}