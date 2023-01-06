#include "regex_lib.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

bool regex_match(const char *pattern, const char *expression){
	regex_t regex;
	int regexec_return;
	
	if (regcomp(&regex, pattern, REG_EXTENDED)) {
		printf("Erro ao compilar uma expressao regular\n");
		exit(EXIT_FAILURE);
	}

	regexec_return = regexec(&regex, expression, 0, NULL, 0);
	regfree(&regex);
	
	return regexec_return == REG_NOMATCH ? false : true;
}