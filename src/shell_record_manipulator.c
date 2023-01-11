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

void save_shell_command_history(Shell *shell) {
  const int _hst_file_line_size_ = COMMAND_BUF_SIZE + PARAMETERS_BUF_SIZE + 1;
  FILE * command_history_file = fopen(_shell_hst_filename, "r+");

  char command_buffer[_hst_file_line_size_];
  char line_buffer[_hst_file_line_size_], last_line[_hst_file_line_size_];

  if (!command_history_file) {
    printf("Erro ao abrir o arquivo de historico de comandos.\n");
    exit(EXIT_FAILURE);
  }
  
  sprintf(command_buffer, "%s %s", shell->comando, shell->parametro);

  last_line[0] = '\0';

  while (fgets(line_buffer, sizeof(line_buffer), command_history_file)) {
    strncpy(last_line, line_buffer, _hst_file_line_size_);
  }
  
  if (last_line[0] == '\0') {
    fprintf(command_history_file, "%s\n", command_buffer);
  }
  else {
    last_line[ strlen(last_line) - 1 ] = '\0';
    if (strcmp(last_line, command_buffer) != 0) {
      fprintf(command_history_file, "%s\n", command_buffer);
    }
  }


  fclose(command_history_file);
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