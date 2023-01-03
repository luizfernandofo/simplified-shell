#include "shell.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
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
    char hostname_buffer[1023+1];

    shell->qty_env_vars = 0;

    if (gethostname(hostname_buffer, sizeof(hostname_buffer)) != 0) {
        printf("Erro ao obter o nome do host.");
        exit(EXIT_FAILURE);
    }

    add_environment_variable(shell, "HOST", hostname_buffer);
    add_environment_variable(shell, "PRONTO", "~$ ");
    add_environment_variable(shell, "SHELL", "simplified-shell");
}

void shell_exit(Shell *shell) {
    if (shell->env_vars) free(shell->env_vars);
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

    else if(strcmp(shell->comando, "sair") == 0){
        shell_exit(shell);
        exit(EXIT_SUCCESS);
    }

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

void split_command_buffer(char *cmd_buff, Shell *shell) {
  char *token = strtok(cmd_buff, " ");
  strcpy(shell->comando, token);
  token = strtok(NULL, "");
  if (token == NULL)
    return;
  strcpy(shell->parametro, token);
}


void add_environment_variable(Shell *shell, char *name, char *content) {
    int quantity = shell->qty_env_vars;

    shell->env_vars = realloc_env_vars(shell->env_vars, quantity + 1);

    shell->env_vars[quantity] = new_env_var(name, content);

    shell->qty_env_vars++;

    // Manipular o arquivo .meushell.rec
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
    // Sempre que a variável DTA for alterada, atualizar
    // a variável PRONTO, de modo que PRONTO=“$DTA \> ”
    return;
}

static void limpa(){

    system("clear");

    return;
}

