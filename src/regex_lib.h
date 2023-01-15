#ifndef REGEX_LIB_H
#define REGEX_LIB_H

#include <stdbool.h>

/**
 * @brief Busca um padrão em uma expressão em formato de string aplicando regex, 
 * retornando 'true' caso seja encontrado e 'false' caso contrário
 * 
 * @param pattern padrão a ser buscado na expressão
 * @param expression expressão
 * @return true 
 * @return false 
 */
bool regex_match(const char *pattern, const char *expression);

#endif