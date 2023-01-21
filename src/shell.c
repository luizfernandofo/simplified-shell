#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "definitions.h"
#include "shell_record_manipulator.h"
#include "regex_lib.h"

// ===================================================

// PADRÕES DE REGEX

// Padrão do conteúdo de uma variável de ambiente
const char *_env_var_get_content_patt_ = "^\\$[a-zA-Z]+$";

// Padrão do comando de configuração de uma variável de ambiente
// amb <var_name>="<spaced_content>"
const char * _full_set_env_var_patt_ = "^amb[ ][A-Za-z]+=\\\"[^\n\\\"]+\\\"$";

// Padrão do parâmetro de configuração de uma variável de ambiente (conteúdo sem espaços)
// <var_name>=<content>
const char *_set_env_var_patt_ = "^[a-zA-Z]+\\=[^\n \\\"]+$";

// Padrão do parâmetro de configuração de uma variável de ambiente (conteúdo com espaços)
// <var_name>="<spaced_content>"
const char *_spaced_set_env_var_patt_ = "^[A-Za-z]+=\\\"[^\n\\\"]+\\\"$";


// ===================================================

// COMANDOS DO SHELL - Definição

/**
 * @brief Exibe o manual de uso do shell
 * 
 */
static void ajuda();


/**
 * @brief Gerencia as variáves de ambiente do shell
 * 
 * @param shell 
 */
static void amb(Shell *shell);


/**
 * @brief Muda para o diretório <dir> atualizando o DTA
 * 
 * @param shell 
 */
static void cd(Shell *shell);


/**
 * @brief Limpa o terminal
 * 
 */
static void limpa();


/**
 * @brief Executa um comando externo
 * 
 * @param shell 
 */
static void executa_comando_externo(Shell *shell);

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
    shell->env_vars_quantity = 0;

    if (gethostname(hostname_buffer, ENV_VAR_CONTENT_BUF_SIZE) != 0){
        printf("Erro ao obter o nome do host.");
        exit(EXIT_FAILURE);
    }
    
    char * temp_buffer = (char *) malloc(sizeof(strlen(hostname_buffer) + 1));

    strncpy(temp_buffer, hostname_buffer, strlen(hostname_buffer) + 1);
    //sprintf(prompt_buffer, "%s:%s$ ", temp_buffer, getenv("PWD"));

    free(temp_buffer);
  
    add_environment_variable(shell, "HOST", hostname_buffer);
    add_environment_variable(shell, "PRONTO", "$DTA /> ");
    add_environment_variable(shell, "SHELL", "simplified-shell");
    add_environment_variable(shell, "DTA", getenv("PWD"));

    strcpy(shell->first_opened_working_dir, get_env_var_content(shell, "DTA"));

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
        // Encerra e sai do shell
        save_shell_command_history(shell);
        shell_exit(shell);
        exit(EXIT_SUCCESS);
    }
    else
        executa_comando_externo(shell);

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

  // strcpy
  
  if (regex_match(_full_set_env_var_patt_, cmd_buff)) {

    int i = 0;
    while (true) {
        cmd_buff[i] = cmd_buff[i + 4];
        if (cmd_buff[i + 4] == '\0') break;
        i++;
    }

    strncpy(shell->comando, "amb", COMMAND_BUF_SIZE);
    strncpy(shell->parametro, cmd_buff, PARAMETERS_BUF_SIZE);

    return;
  }

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
    int quantity = shell->env_vars_quantity;

    shell->env_vars = realloc_env_vars(shell->env_vars, quantity + 1);

    shell->env_vars[quantity] = new_env_var(name, content);

    shell->env_vars_quantity++;

    // Manipular o arquivo .meushell.rec
}


bool has_env_var(Shell *shell, char *name) {
    int i;
    for (i = 0; i < shell->env_vars_quantity; i++) {
        if ( !strcmp(shell->env_vars[i].name, name) ) 
            return true;
    }
    return false;
}


bool set_env_var_content(Shell *shell, char *name, char *content) {
    int i;
    for (i = 0; i < shell->env_vars_quantity; i++) {
        if ( !strcmp(shell->env_vars[i].name, name) ) {
          strncpy(shell->env_vars[i].content, content, ENV_VAR_CONTENT_BUF_SIZE);
          return true;
        }
    }
    return false;
}


char* get_env_var_content(Shell *shell, char *name) {
    int i, j, k, l;
    char *temp_ptr = NULL;
    char temp_buff[1024];
    char var[256];

    //shell->env_var_content_resolved_buffer[0] = '\0';

    for(i = 0; i < shell->env_vars_quantity; i++) {
        if ( !strcmp(shell->env_vars[i].name, name) ){
          
          temp_ptr = shell->env_vars[i].content;

          // Verifica se há variáveis de ambiente no content
          for(j = 0; j < ENV_VAR_CONTENT_BUF_SIZE && temp_ptr[j] != '\0'; j++){

            if(temp_ptr[j] == '$'){

              strncpy(temp_buff, temp_ptr, j);
              
              temp_buff[j] = '\0';

              for(l = j + 1, k = 0; l < ENV_VAR_CONTENT_BUF_SIZE && temp_ptr[l] != '\0' && k < 256; l++, k++){

                if(temp_ptr[l] == ' '){
                  break;
                }
                var[k] = temp_ptr[l];
              }

              var[k] = '\0';
              
              temp_ptr += l;
              
              if(has_env_var(shell, var)){
                strcat(temp_buff, get_env_var_content(shell, var));
              }

              strcat(temp_buff, temp_ptr);

              strcpy(shell->env_var_content_resolved_buffer, temp_buff);

              return shell->env_var_content_resolved_buffer;
            }
          }

          return shell->env_vars[i].content;
          
        } 
            
    }
    return NULL;
}

// ===================================================

// COMANDOS DO SHELL - Implementação

static void ajuda(){
    printf("\n----------AJUDA----------\n");
    printf("Uso: <comando> <parametro>\n\n");
    printf("Comandos:\n");
    printf("$ <comando_externo> <param> <param> ...\n");
    printf("$ ajuda\n");
    printf("$ amb\n");
    printf("$ amb $<variavel>\n");
    printf("$ amb <variavel>=<valor>\n");
    printf("$ cd <diretorio>\n");
    printf("$ limpa\n");
    printf("$ sair\n\n");
}


static void amb(Shell *shell) {
  char params[PARAMETERS_BUF_SIZE];
  strncpy(params, shell->parametro, PARAMETERS_BUF_SIZE);
  bool match1, match2;

  // $ amb
  if (*(params) == '\0') {
    int index;
    printf("-----------------------------\n");
    printf("%-10s | %-15s\n", "VAR NAME", "VAR CONTENT");
    printf("-----------------------------\n");
    for (index = 0; index < shell->env_vars_quantity; index++) {
      printf("%-10s | ", shell->env_vars[index].name);
      printf("%-15s\n", shell->env_vars[index].content);
    }
    printf("-----------------------------\n");
  }
  // $ amb $VAR
  else if (regex_match(_env_var_get_content_patt_, params)) {
    char *var_name = strtok(params, "$");
    char *var_content = NULL;
    if (!has_env_var(shell, var_name))
      printf("Variavel de ambiente nao encontrada\n");
    else{
      for(int i=0; i < shell->env_vars_quantity; i++){
        if(strcmp(var_name, shell->env_vars[i].name) == 0){
          printf("%s=%s\n", var_name, shell->env_vars[i].content);
          break;
        }
      }
    }
      
  }
  // $ amb VAR=<value>
  else if ( (match1 = regex_match(_set_env_var_patt_, params)) || 
              (match2 = regex_match(_spaced_set_env_var_patt_, params)) ) {

    char *var_name = strtok(params, "=");
    char *var_content = strtok(NULL, (match1 ? "" : "\""));

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
    
	struct stat sb;

  char cwd[ENV_VAR_CONTENT_BUF_SIZE];

	if(stat(shell->parametro, &sb) == 0 && S_ISDIR(sb.st_mode)) {

		if(chdir(shell->parametro)){

			printf("Erro alterando working dir.\n");

			shell_exit(shell);
			
			exit(EXIT_FAILURE);
		}
    }
	else {
        printf("O diretorio informado nao existe.\n");
    }

    set_env_var_content(shell, "DTA", getcwd(cwd, ENV_VAR_CONTENT_BUF_SIZE));

    return;
}


static void limpa(){

    system("clear");

    return;
}


static void executa_comando_externo(Shell *shell){

	char *empty_string = "";
	char path[31+1] = "/bin/";
	char *params[32];
	char *parametro = shell->parametro;
	int i = 1;

	params[0] = empty_string;

	params[1] = strtok(parametro, " ");

	while(params[i] && i < 32)
	{
		i++;

		params[i] = strtok(NULL, " ");
	}

	strcat(path, shell->comando);

	if(fork()==0){
		if(execv(path, params) == -1){

			printf("%s: Comando nao encontrado.\n", shell->comando);

			shell_exit(shell);
			exit(EXIT_FAILURE);
		}
	}
    else{
		usleep(150000);
    }
}
