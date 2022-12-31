#ifndef SHELL_RECORD_MANIPUTALOR_H
#define SHELL_RECORD_MANIPUTALOR_H

#include "shell.h"
#include "env_var.h"

EnvironmentVariable* load_shell_env_vars();

void save_shell_env_vars(Shell* shell);

void save_shell_command_history(Shell* shell);

#endif