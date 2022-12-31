#ifndef ENV_VAR_H
#define ENV_VAR_H

#include "definitions.h"

typedef struct EnvironmentVariable{

    char name[ENV_VAR_NAME_BUF_SIZE];
    char content[ENV_VAR_CONTENT_BUF_SIZE];

}EnvironmentVariable;

#endif