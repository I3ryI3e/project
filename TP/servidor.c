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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "servidor.h"
#include "dados.h"

int main(int argc, char** argv){

    FILE *f;
    char comando[100];
    /*int openfile;

    if(mkfifo("fifoserv", S_IWUSR | S_IRUSR) != 0)
        return(EXIT_FAILURE);
    
    if((openfile = open("fifoserv", O_RDONLY)) < 0){
        printf("Erro ao abrir o ficheiro\n");
        return;       
    }*/
    
    if(argc != 2){
        printf("Sintaxe: %s nome_do_ficheiro\n", argv[0]);
        return (EXIT_FAILURE);
    }
    if((f = fopen(argv[1], "rt")) == NULL){
        printf("Erro: Ficheiro nao encontrado\n");
        return (EXIT_FAILURE);
    }
    fclose(f);
    printf("Mensagem de boas vindas..\n");
    while(1){
        scanf(" %[^\n]99s", comando);
        
        if(!strncmp(comando, "add ", 4)){
            adduser(argv[1], comando);
            printf("Tudo bem\n");
            continue;
        }
        if(!strncmp(comando, "users", 5)){
            printf("Mostra users\n");
            continue;
        }
        if(!strncmp(comando, "kick ", 5)){
            printf("Termina a ligacao do jogador\n");
            continue;
        }
        if(!strncmp(comando, "game", 4)){
            printf("Mostra informacao sobre o jogo\n");
            continue;
        }
        if(!strncmp(comando, "shutdown", 8)){
            printf("Vai sair\n");
            return (EXIT_SUCCESS);
        }
        if(!strncmp(comando, "map ", 4)){
            printf("Carrega um labirinto\n");
            continue;
        }
        if(!strncmp(comando, "help", 4)){
            printf("\nComandos reconhecidos:\n\tadd username password;\n\tusers;\n\tkick username;\n\tgame;\n\tshutdown;\n\tmap nome_do_ficheiro;\n");
            continue;
        }
        printf("\nComando nao reconhecido, escreva 'help' para ver os comandos\n");
    }
}

void adduser(char* nomefich, char* cmd){
    
    FILE *f;
    clicom new, aux;
    int i, j, flag=0; //file;
    
    for(i=4, j=0;i<100;i++, j++){
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
    /*
    if((file = open(nomefich, O_RDONLY)) < 0){
        printf("Erro ao abrir o ficheiro\n");
        return;       
    }
    while(read(file, &aux, sizeof(aux)) != 0){
        if(strcmp(new.username, aux.username) == 0){
            printf("Erro: nome do utilizador ja existente\n");
            close(file);
            return;
        }
    }
    close(file);
    if((file = open(nomefich, O_WRONLY | O_APPEND)) < 0){
        printf("Erro ao abrir o ficheiro\n");
        return;       
    }
    if(write(file, &new, sizeof(aux)) != sizeof(aux)){
        close(file);
        printf("Erro na escrita do novo utilizador\n");
        return;
    }
    close(file);*/
    
    if((f = fopen(nomefich, "a+")) == NULL){
        printf("Erro ao abrir o ficheiro\n");
        return;
    }
    while(fscanf(f, " %29s", aux.username) != EOF){
        fscanf(f, " %29s", aux.password);
        if(strcmp(new.username, aux.username) == 0){
            printf("Erro: nome do utilizador ja existente\n");
            fclose(f);
            return;
        }
    }
    fprintf(f, "%s\n%s\n", new.username, new.password);
    
    fclose(f);
    return;
}