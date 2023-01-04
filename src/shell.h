#ifndef SHELL_H
#define SHELL_H

#include "env_var.h"
#include "definitions.h"

/*
Shell deve conter os seguintes comandos:

- ajuda - exibe o manual de uso do shell
- amb - lista as variáves de ambiente do shell
- cd <dir> - muda para o diretório <dir> atualizando o DTA
- <comando> - executa comando externo usando fork() e exec()
- limpa - limpa o terminal
- sair - sai do shell

*/

typedef struct Shell{

    char DTA[DTA_BUF_SIZE];

    char comando[COMMAND_BUF_SIZE];

    char parametro[PARAMETERS_BUF_SIZE];

    EnvironmentVariable* env_vars;

    int qty_env_vars;

} Shell;


void shell_setup(Shell *shell);

void shell_exit(Shell *shell);

void shell_clear(Shell *shell);

void eval_command(Shell *shell);

void read_string(char *str);

void split_command_buffer(char *cmd_buff, Shell *shell);

void add_environment_variable(Shell *shell, char *name, char *content);

char* get_env_var_content(Shell *shell, char *name);

#endif