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

    char cmd_buff[COMMAND_BUF_SIZE + PARAMETERS_BUF_SIZE];

    FILE *file = NULL;

    shell_setup(&shell);

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

                shell_clear(&shell);

                cmd_buff[0] = '\0';
            }

            fclose(file);
        }

        return 0;

    }

    while(true){

        // nome exibido pelo shell imediatamente antes do cursor
        printf("%s", shell.env_vars[1].content);

        read_string(cmd_buff);

        split_command_buffer(cmd_buff, &shell);

        eval_command(&shell);

        shell_clear(&shell);

        cmd_buff[0] = '\0';
    }
    

    return 0;
}
