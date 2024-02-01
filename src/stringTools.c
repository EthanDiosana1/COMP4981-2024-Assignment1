//
// Created by main on 28/01/24.
//
#include "stringTools.h"

char *getLastToken(const char *string, const char *delim) {
  char *strcopy;
  char *token;
  char *lastToken = NULL;
  char *savePtr;

  // Duplicate the string.
  strcopy = strdup(string);

  // Tokenize the string.
  token = strtok_r(strcopy, delim, &savePtr);

  // Get the last token.
  while (token) {
    lastToken = token;
    token = strtok_r(NULL, delim, &savePtr);
  }

  // Free.
  free(strcopy);

  return lastToken;
}

#include <stdlib.h>
#include <string.h>

int getNumberOfTokens(const char *string, const char *delim) {
  int count = 0;
  char *strcopy;
  const char *token;
  char *savePtr;

  // Duplicate the string.
  strcopy = strdup(string);

  // Initialize count to 0;
  token = strtok_r(strcopy, delim, &savePtr);

  // Count until no more tokens.
  while (token) {
    count++;
    token = strtok_r(NULL, delim, &savePtr);
  }

  // Free duplicated string.
  free(strcopy);

  return count;
}

// char *strTokToArray(const char *string, const char *delim) {
//   char *strcopy = NULL;
//   char *token = NULL;
//   char **tokenArray;
//   int numTokens;
//   int count;
//
//   // Get the number of tokens.
//   numTokens = getNumberOfTokens(string, delim);
//
//   // Allocate the memory for the token array.
//   tokenArray = (char **)malloc(sizeof(char *) * (unsigned long) numTokens);
//
//   // Tokenize and store in the array.
//
//   // Free.
//   free(strcopy);
//   free(token);
//
//   return tokenArray;
// }
