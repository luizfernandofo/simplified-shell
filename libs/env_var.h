#ifndef ENV_VAR_H
#define ENV_VAR_H

typedef struct EnvironmentVariable{

    char name[31+1];
    char content[127+1];

}EnvironmentVariable;

EnvironmentVariable* EnvVarStack;



#endif