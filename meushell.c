#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "src/env_var.h"
#include "src/shell.h"
#include "src/shell_record_manipulator.h"
#include "src/definitions.h"

int main(int argc, char const *argv[])
{

    Shell shell;

    // Buffer que armazena a linha de comando (comando e seus parâmetros)
    char cmd_buff[COMMAND_BUF_SIZE + PARAMETERS_BUF_SIZE];

    FILE *file = NULL;

    // Inicialização do shell
    shell_setup(&shell);

    // Leitura do arquivo de extensão .cmds para a execução dos comandos
    if(argc > 1){

        if(strstr(argv[1], ".cmds")){

            file = fopen(argv[1], "r");
            
            if(file == NULL){
                printf("Arquivo %s nao existe.\nUso: ./meushell {nome_arquivo}.cmds\n", argv[1]);
                return EXIT_FAILURE;
            }


            while(read_line(cmd_buff, file)){

                split_command_buffer(cmd_buff, &shell);

                eval_command(&shell);

                save_shell_command_history(&shell);

                shell_clear(&shell);

                cmd_buff[0] = '\0';
            }

            fclose(file);
        }

        return 0;

    }

    // Shell como um terminal interativo
    while(true){

        // Nome exibido pelo shell imediatamente antes do cursor
        printf("%s", shell.env_vars[1].content);

        // Leitura da linha de comando pelo teclado
        read_string(cmd_buff);

        // Separação do comando de seus parâmetros
        split_command_buffer(cmd_buff, &shell);

        // Análise do comando informado
        eval_command(&shell);

        // Salvamento do comando no histórico
        save_shell_command_history(&shell);

        // Reinicialização/limpeza do shell
        shell_clear(&shell);

        // Limpeza do buffer da linha de comando
        cmd_buff[0] = '\0';
    }
    

    return 0;
}
