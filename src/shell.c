#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "definitions.h"
#include "shell_record_manipulator.h"
#include "regex_lib.h"


// ===================================================

// COMANDOS DO SHELL

static void ajuda();

static void amb(Shell *shell);

static void cd(Shell *shell);

static void limpa();

// ===================================================

// Função para DEBUG do conteúdo de memória de uma string
void dump(void *p, int n) {

  unsigned char *p1 = p;

  n++;

  while (n--) {

    printf("%c%d", *p1, *p1);

    p1++;

  }
  printf("\n");

}

void shell_setup(Shell *shell){
    char hostname_buffer[ENV_VAR_CONTENT_BUF_SIZE];
    char prompt_buffer[ENV_VAR_CONTENT_BUF_SIZE];

    shell->env_vars = NULL;
    shell->qty_env_vars = 0;

    if (gethostname(hostname_buffer, sizeof(hostname_buffer)) != 0) {
        printf("Erro ao obter o nome do host.");
        exit(EXIT_FAILURE);
    }
    

    sprintf(prompt_buffer, "%s:%s$ ", hostname_buffer, getenv("HOME"));
  
    add_environment_variable(shell, "HOST", hostname_buffer);
    add_environment_variable(shell, "PRONTO", prompt_buffer);
    add_environment_variable(shell, "SHELL", "simplified-shell");
    add_environment_variable(shell, "DTA", getenv("HOME"));
}

void shell_exit(Shell *shell) {
    if (shell->env_vars) free(shell->env_vars);
}

void shell_clear(Shell *shell) {
  shell->comando[0] = '\0';
  shell->parametro[0] = '\0';
}

void eval_command(Shell *shell){

    //const char* shell_comands[] = {"ajuda", "amb", "cd", "limpa", "sair"};

    if(strcmp(shell->comando, "ajuda") == 0 || strcmp(shell->comando, "help") == 0)
        ajuda();

    else if(strcmp(shell->comando, "amb") == 0)
        amb(shell);

    else if(strcmp(shell->comando, "cd") == 0)
        cd(shell);

    else if(strcmp(shell->comando, "limpa") == 0 || strcmp(shell->comando, "clear") == 0)
        limpa();

    else if(strcmp(shell->comando, "sair") == 0 || strcmp(shell->comando, "exit") == 0){
        shell_exit(shell);
        exit(EXIT_SUCCESS);
    }
    else
        printf("Execucao de comando externo.\n");

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
  // char token[COMMAND_BUF_SIZE + PARAMETERS_BUF_SIZE];

  // strncpy(token, cmd_buff, (COMMAND_BUF_SIZE + PARAMETERS_BUF_SIZE));

  char *token = cmd_buff;

  int i=0;
  while(token[i] != '\0'){

  if(token[i] == ' '){

      token[i] = '\0';
      i++;
      break;

    }

    i++;
  }

  strncpy(shell->comando, token, COMMAND_BUF_SIZE);

  strncpy(shell->parametro, &token[i], PARAMETERS_BUF_SIZE);

}

void add_environment_variable(Shell *shell, char *name, char *content) {
    int quantity = shell->qty_env_vars;

    shell->env_vars = realloc_env_vars(shell->env_vars, quantity + 1);

    shell->env_vars[quantity] = new_env_var(name, content);

    shell->qty_env_vars++;

    // Manipular o arquivo .meushell.rec
}

bool has_env_var(Shell *shell, char *name) {
    int i;
    for (i = 0; i < shell->qty_env_vars; i++) {
        if ( !strcmp(shell->env_vars[i].name, name) ) 
            return true;
    }
    return false;
}


bool set_env_var_content(Shell *shell, char *name, char *content) {
    int i;
    for (i = 0; i < shell->qty_env_vars; i++) {
        if ( !strcmp(shell->env_vars[i].name, name) ) {
          strncpy(shell->env_vars[i].content, content, ENV_VAR_CONTENT_BUF_SIZE);
          return true;
        }
    }
    return false;
}


char* get_env_var_content(Shell *shell, char *name) {
    int i;
    for (i = 0; i < shell->qty_env_vars; i++) {
        if ( !strcmp(shell->env_vars[i].name, name) ) 
            return shell->env_vars[i].content;
    }
    return NULL;
}

// ===================================================

// COMANDOS DO SHELL

static void ajuda(){
    printf("\n----------AJUDA----------\n");
    printf("Uso: <comando> <parametro>\n\n");
    printf("Comandos:\n");
    printf("$ <comando_externo>\n");
    printf("$ ajuda\n");
    printf("$ amb\n");
    printf("$ amb <variavel>\n");
    printf("$ amb <variavel>=<valor>\n");
    printf("$ cd <diretorio>\n");
    printf("$ limpa\n");
    printf("$ sair\n\n");
}

static void amb(Shell *shell) {
  const char *env_var_content_patt = "^\\$[a-zA-Z]+$";
  const char *set_env_var_patt = "^[a-zA-Z]+\\=[a-zA-Z]+$";

  char *params = shell->parametro;


  // $ amb
  if (*(params) == '\0') {
    int index;
    printf("-----------------------------\n");
    printf("%-10s | %-15s\n", "VAR NAME", "VAR CONTENT");
    printf("-----------------------------\n");
    for (index = 0; index < shell->qty_env_vars; index++) {
      printf("%-10s | ", shell->env_vars[index].name);
      printf("%-15s\n", shell->env_vars[index].content);
    }
    printf("-----------------------------\n");
  }
  // $ amb $VAR
  else if (regex_match(env_var_content_patt, params)) {
    char *var_name = strtok(params, "$");
    char *var_content = get_env_var_content(shell, var_name);
    if (var_content == NULL)
      printf("Variavel de ambiente nao encontrada\n");
    else
      printf("%s=%s\n", var_name, var_content);
  }
  // $ amb VAR=<value>
  else if (regex_match(set_env_var_patt, params)) {
    char *var_name = strtok(params, "=");
    char *var_content = strtok(NULL, "");
    if (has_env_var(shell, var_name)) {
      set_env_var_content(shell, var_name, var_content);
      printf("Variavel de ambiente configurada\n");
    }
    else {
      add_environment_variable(shell, var_name, var_content);
      printf("Variavel de ambiente adicionada\n");
    }
  }
  // Comando invalido
  else {
    printf("Comando invalido\n");
    //exit(EXIT_FAILURE); // Temporário
  }

  return;
}

static void cd(Shell *shell){
    // Sempre que a variável DTA for alterada, atualizar
    // a variável PRONTO, de modo que PRONTO=“$DTA \> ”
    return;
}

static void limpa(){

    system("clear");

    return;
}

