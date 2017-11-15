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
    
    /*for(i=4, j=0;i<100;i++, j++){
        if(!flag){
            if(cmd[i] != ' '){
                new.username[j] = cmd[i];
            }else{
                new.username[j] = '\0';
                flag=1;
                j=0;
                i++;
            }
        }
        if(flag){
            if(cmd[i] != '\0'){
                new.password[j] = cmd[i];
            }else{
                new.password[j] = cmd[i];
                i=100;
            }
        }
    }
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
    close(file);*/
    
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

void tratateclado(){
    
    char* cmd;
    char linha[100], token[100];
    
    scanf(" %[^\n]99s", linha);
        
    strncpy(token, linha, 99);
    cmd = strtok(token, " ");

    if(!strcmp(cmd, "add")){
        adduser("data.txt", linha);
        printf("Tudo bem\n");
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
                        shutdown(0);
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

void shutdown(int sig){
    
    close(3);               //NAO PODE SER 3
    unlink("fifologin");
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv){

    FILE *f;
    int openfile, rs;
    fd_set readfds;
    
    if(signal(SIGUSR1, shutdown) == SIG_ERR){
        perror("Erro no sinal\n");
        return(EXIT_FAILURE);
    }
    
    if(argc != 1){
        printf("Sintaxe: %s nome_do_ficheiro\n", argv[0]);
        return (EXIT_FAILURE);
    }
    if((f = fopen("data.txt", "rt")) == NULL){
        perror("Erro: Ficheiro nao encontrado\n");
        return (EXIT_FAILURE);
    }
    fclose(f);
    
    if(mkfifo("fifologin", S_IWUSR | S_IRUSR) != 0)
        return(EXIT_FAILURE);
    
    if((openfile = open("fifologin", O_RDWR)) < 0){
        perror("Erro ao abrir o ficheiro\n");
        return(EXIT_FAILURE);
    }
    
    printf("Mensagem de boas vindas..\n");
    
    while(1){
        FD_ZERO(&readfds);
        FD_SET(openfile, &readfds);
        FD_SET(0, &readfds);
        rs = select(openfile+1, &readfds, NULL, NULL, NULL);
        if(rs == -1)
            perror("Erro no select\n");
        if(FD_ISSET(0, &readfds))
            tratateclado();
        if(FD_ISSET(openfile, &readfds)){
            printf("OLHO FIFOO");
        }
    }
}