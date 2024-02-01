//
// Created by main on 28/01/24.
//

#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

#include <stdlib.h>
#include <string.h>

/**
 * @brief Returns the final token of a string to be tokenized.
 * @param string The string to tokenize.
 * @param delim The delimiter.
 * @return last token
 */
char *getLastToken(const char *string, const char *delim);

/**
 * @brief Returns the number of tokens from the string.
 * @param string The string to tokenize.
 * @param delim The delimiter.
 * @return number of tokens
 */
int getNumberOfTokens(const char *string, const char *delim);

/**
 * @brief Returns an array of strings from a tokenized string.
 * @param string The string to tokenize.
 * @param delim The delimiter.
 * @return array
 */
char *strTokToArray(const char *string, const char *delim);

#endif // STRINGTOOLS_H
