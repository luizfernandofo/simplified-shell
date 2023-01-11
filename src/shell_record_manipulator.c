#include "shell_record_manipulator.h"

#include <stdio.h>
#include <string.h>
#include "definitions.h"
#include <stdlib.h>

static const char* _shell_rec_filename = ".meushell.rec";
static const char* _shell_hst_filename = ".meushell.hst";


EnvironmentVariable *load_shell_env_vars()
{
    EnvironmentVariable *env_var = NULL;

    FILE *shell_rec_file = NULL;

    long int file_size = 0;
     
    unsigned int qtd_env_vars = 0;

    shell_rec_file = fopen(_shell_rec_filename, "r+");
    
    if(shell_rec_file == NULL){
        return NULL;    
    }

    fseek(shell_rec_file, 0, SEEK_END);

    file_size = ftell(shell_rec_file);

    qtd_env_vars = (unsigned int) file_size/sizeof(EnvironmentVariable);

    env_var = (EnvironmentVariable *) malloc(file_size);

    if(env_var == NULL){
        printf("Erro alocando memoria.\n");
        exit(EXIT_FAILURE);
    }

    fseek(shell_rec_file, 0, SEEK_SET);

    int fread_return = fread((void *) env_var, sizeof(EnvironmentVariable), qtd_env_vars, shell_rec_file);

    fclose(shell_rec_file);

    return env_var;

}

void save_shell_env_vars(Shell *shell)
{
}

void save_shell_command_history(Shell *shell)
{
}

void remove_newline(char *ptr){
  while (*ptr) {
    if (*ptr == '\n')
      *ptr = 0;
    else
      ptr++;
  }
}


int read_line(char *line, FILE *file){
    if(fgets(line, COMMAND_BUF_SIZE + PARAMETERS_BUF_SIZE, file) != NULL){
        remove_newline(line);

        return 1;
    }
    else return 0;
}