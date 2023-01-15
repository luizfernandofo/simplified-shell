#ifndef ENV_VAR_H
#define ENV_VAR_H

#include "definitions.h"

/**
 * @brief Estrutura de uma variável de ambiente
 * 
 * @attr (char) name: nome da variável
 * @attr (char) content: conteúdo da variável
 */
typedef struct EnvironmentVariable {
    char name[ENV_VAR_NAME_BUF_SIZE];
    char content[ENV_VAR_CONTENT_BUF_SIZE];
} EnvironmentVariable;


/**
 * @brief Cria uma nova variável de ambiente
 * 
 * @param name nome da variável
 * @param content conteúdo da variável
 * @return EnvironmentVariable 
 */
EnvironmentVariable new_env_var(char *name, char *content);


/**
 * @brief Gerencia a alocação de uma variável de ambiente na memória
 * 
 * @param pointer 
 * @param quantity 
 * @return EnvironmentVariable* 
 */
EnvironmentVariable *realloc_env_vars(EnvironmentVariable *pointer, int quantity);

#endif