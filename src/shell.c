#include "shell.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "definitions.h"
#include "shell_record_manipulator.h"

// ===================================================

// COMANDOS DO SHELL

static void ajuda();

static void amb(const char *params);

static void cd(const char *DTA);

static void limpa();

// ===================================================

void shell_setup(Shell *shell){

    return;
}

void eval_command(Shell *shell){

    //const char* shell_comands[] = {"ajuda", "amb", "cd", "limpa", "sair"};

    if(strcmp(shell->comando, "ajuda") == 0)
        ajuda();

    else if(strcmp(shell->comando, "amb") == 0)
        amb(shell->parametro);

    else if(strcmp(shell->comando, "cd") == 0)
        cd(shell->DTA);

    else if(strcmp(shell->comando, "limpa") == 0)
        limpa();

    else if(strcmp(shell->comando, "sair") == 0)
        exit(EXIT_SUCCESS);

    else
        printf("Execucao de comando externo.");

    return;
}

void read_string(char *str) {
  int i = 0;
  char c;
  while ((c = getchar()) != '\n' && i < (COMMAND_BUF_SIZE + PARAMETERS_BUF_SIZE - 1)) {
    str[i] = c;
    i++;
  }
  str[i] = '\0';
}


// ===================================================

// COMANDOS DO SHELL

static void ajuda(){
    printf("----------AJUDA----------\n");
    printf("Uso: <comando> <parametro>\n\n");
    printf("Comandos:\n");
    printf("ajuda\n");
    printf("amb\n");
    printf("<comando externo>\n");
    printf("cd\n");
    printf("limpa\n");
    printf("sair\n");
}

static void amb(const char *params){
    return;
}

static void cd(const char *DTA){
    return;
}

static void limpa(){

    system("clear");

    return;
}

