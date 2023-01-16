#ifndef SHELL_H
#define SHELL_H

#include "env_var.h"
#include "definitions.h"
#include <stdbool.h>

/**
 * Shell deve conter os seguintes comandos:
 * 
 * ajuda - exibe o manual de uso do shell
 * amb - gerencia as variáves de ambiente do shell
 * cd <dir> - muda para o diretório <dir> atualizando o DTA
 * <comando> - executa comando externo usando fork() e exec()
 * limpa - limpa o terminal
 * sair - sai do shell
 */


/**
 * @brief Estrutura que armazena os dados essenciais do shell
 * 
 * @attr (char) comando: buffer do comando a ser executado
 * @attr (char) parametro: buffer dos parâmetros passados para o comando
 * @attr (EnvironmentVariable*): conjunto de variáveis de ambiente
 * @attr (int) env_vars_quantity: quantidade de variáveis de ambiente
 * @attr (char) first_opened_working_dir: primeiro diretório de trabalho aberto
 */
typedef struct Shell{

    char comando[COMMAND_BUF_SIZE];

    char parametro[PARAMETERS_BUF_SIZE];

    EnvironmentVariable* env_vars;

    int env_vars_quantity;

    char first_opened_working_dir[1024];

} Shell;

/**
 * @brief Inicializa o shell realizando as configurações básicas e 
 * adicionando as variáveis de ambiente built-in
 * 
 * @param shell 
 */
void shell_setup(Shell *shell);


/**
 * @brief Libera a memória dinâmica alocada pelo shell
 * 
 * @param shell 
 */
void shell_exit(Shell *shell);


/**
 * @brief Realiza a limpeza dos buffers de comando e parâmetros
 * 
 * @param shell 
 */
void shell_clear(Shell *shell);


/**
 * @brief Analisa a linha de comando para então chamar as funções 
 * referentes ao comando solicitado
 * 
 * @param shell 
 */
void eval_command(Shell *shell);


/**
 * @brief Realiza a letura de uma string até uma quebra de linha ('\n')
 * 
 * @param str string a ser lida
 */
void read_string(char *str);


/**
 * @brief Separa o comando de seus parâmetros, armazenado 
 * ambos na estrutura do shell
 * 
 * @param cmd_buff buffer da linha de comando
 * @param shell 
 */
void split_command_buffer(char *cmd_buff, Shell *shell);


/**
 * @brief Adiciona uma nova variável de ambiente
 * 
 * @param shell 
 * @param name nome da variável
 * @param content conteúdo da variável
 */
void add_environment_variable(Shell *shell, char *name, char *content);


/**
 * @brief Retorna o conteúdo de uma variável de ambiente cujo 
 * nome é informado como parâmetro
 * 
 * @param shell 
 * @param name nome da variável de ambiente
 * @return char* 
 */
char* get_env_var_content(Shell *shell, char *name);


/**
 * @brief Verifica se uma variável de ambiente cujo nome é informado como 
 * parâmetro existe ou não
 * 
 * @param shell 
 * @param name nome da variável de ambiente
 * @return true 
 * @return false 
 */
bool has_env_var(Shell *shell, char *name);


/**
 * @brief Configura o valor de uma variável de ambiente
 * 
 * @param shell 
 * @param name nome da variável de ambiente
 * @param content conteúdo da variável de ambiente
 * @return true 
 * @return false 
 */
bool set_env_var_content(Shell *shell, char *name, char *content);

#endif