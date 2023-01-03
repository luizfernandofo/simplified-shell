#include "env_var.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

EnvironmentVariable new_env_var(char *name, char *content) {
    EnvironmentVariable env_var;
    strcpy(env_var.name, name);
    strcpy(env_var.content, content);
    return env_var;
}

EnvironmentVariable * realloc_env_vars(EnvironmentVariable *pointer, int quantity) {
    EnvironmentVariable *env_vars = NULL;

    env_vars = (EnvironmentVariable *) realloc(pointer, quantity * sizeof(EnvironmentVariable));

    if (env_vars == NULL) {
        printf("Erro na realocacao de variaveis de ambiente");
        exit(EXIT_FAILURE);
    }

    return env_vars;
}