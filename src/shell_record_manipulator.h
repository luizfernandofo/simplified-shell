#ifndef SHELL_RECORD_MANIPUTALOR_H
#define SHELL_RECORD_MANIPUTALOR_H

#include "shell.h"
#include "env_var.h"

#include <stdio.h>

/**
 * @brief 
 * 
 * @return EnvironmentVariable* 
 */
EnvironmentVariable* load_shell_env_vars();


/**
 * @brief 
 * 
 * @param shell 
 */
void save_shell_env_vars(Shell* shell);


/**
 * @brief Salva os comandos utilizados em um histórico, um
 * arquivo de texto nomeado .meushell.hst
 * 
 * @param shell 
 */
void save_shell_command_history(Shell* shell);


/**
 * @brief 
 * 
 * @param ptr 
 */
void remove_newline(char *ptr);


/**
 * @brief 
 * 
 * @param line 
 * @param file 
 * @return int 
 */
int read_line(char *line, FILE *file);

#endif