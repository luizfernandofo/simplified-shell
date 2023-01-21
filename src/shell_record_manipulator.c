#include "shell_record_manipulator.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "regex_lib.h"
#include "definitions.h"
#include "shell.h"

static const char* _shell_rec_filename = ".meushell.rec";
static const char* _shell_hst_filename = "/.meushell.hst";


void load_shell_env_vars(Shell *shell) {
    FILE *shell_rec_file = NULL;
    const int _rec_file_line_size_ = PARAMETERS_BUF_SIZE;
    char line_buffer[_rec_file_line_size_], current_line[_rec_file_line_size_];

    shell_rec_file = fopen(_shell_rec_filename, "r");
    
    if (shell_rec_file == NULL){
        printf("Arquivo %s inexistente, variaveis de ambiente setadas como default.\n", _shell_rec_filename);
        return;   
    }

  bool set_env_var_match, spaced_set_env_var_match;
  while (fgets(line_buffer, sizeof(line_buffer), shell_rec_file)) {
    if (regex_match("^[ \n]*$", line_buffer)) continue;

    if (line_buffer[ strlen(line_buffer) - 1 ] == '\n')
      line_buffer[ strlen(line_buffer) - 1 ] = '\0';
    
    if ( (set_env_var_match = regex_match("^[a-zA-Z]+\\=[^\n \\\"]+$", line_buffer)) || 
              (spaced_set_env_var_match = regex_match("^[A-Za-z]+=\\\"[^\n]+\\\"$", line_buffer)) ) {

      char *var_name = strtok(line_buffer, "=");
      char *var_content = strtok(NULL, (set_env_var_match ? "" : "\""));

      if (has_env_var(shell, var_name)) 
        set_env_var_content(shell, var_name, var_content);
      else 
        add_environment_variable(shell, var_name, var_content);
    }
    else {

      printf("Erro no conteudo de .meushell.rec.\n\n");

      printf("Uso:\n\nNOME=conteudo_sem_espaco\nNOME=\"conteudo com espacos\"\nPRONTO=\"$PWD /> \"\n\n");

      exit(EXIT_FAILURE);
    }
  }

    fclose(shell_rec_file);

}


void save_shell_command_history(Shell *shell) {

  const int _hst_file_line_size_ = COMMAND_BUF_SIZE + PARAMETERS_BUF_SIZE + 1;

	char file_path[1024];

	// Make use of a absolute path to save the history and prevent errors due to chdir() calls.
	stpcpy(file_path, shell->first_opened_working_dir);

	strcat(file_path, _shell_hst_filename);
	// ----

  FILE * command_history_file = fopen(file_path, "r+");

  char command_buffer[_hst_file_line_size_];
  char line_buffer[_hst_file_line_size_], last_line[_hst_file_line_size_];

  if (command_history_file == NULL) {

  command_history_file = fopen(file_path, "w+");

	if (command_history_file == NULL) {

		printf("Erro ao abrir o arquivo de historico de comandos.\n");
		exit(EXIT_FAILURE);

	}
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
