#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "src/env_var.h"
#include "src/shell.h"
#include "src/shell_record_manipulator.h"
#include "src/definitions.h"

int main(int argc, char const *argv[])
{
    
    Shell shell;

    char cmd_buff[COMMAND_BUF_SIZE + PARAMETERS_BUF_SIZE];

    shell_setup(&shell);

    if(argc > 1){
        // execução de arquivo passado por linha de comando
        printf("Debug: Arquivo passado por linha de comando executado.\n");
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
