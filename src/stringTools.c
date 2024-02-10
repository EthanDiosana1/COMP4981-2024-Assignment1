//
// Created by main on 28/01/24.
//
#include "stringTools.h"
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

TokenAndStr getFirstToken(const char *string, const char *delim) {
  TokenAndStr result;
  char *savePtr;

  // Duplicate the string.
  result.originalStr = strdup(string);

  // Tokenize the string.
  result.token = strtok_r(result.originalStr, delim, &savePtr);

  // Now, the caller is responsible for freeing result.originalStr when done
  // with the token.
  return result;
}

TokenAndStr getLastToken(const char *string, const char *delim) {
  TokenAndStr result;
  char *savePtr;
  char *token;

  // Initialize result to ensure a clean state
  result.originalStr = NULL;
  result.token = NULL;

  // Duplicate the string.
  result.originalStr = strdup(string);
  if (!result.originalStr) {
    // Handle strdup failure, possibly due to memory allocation failure
    return result;
  }

  // Tokenize the string and find the last token.
  for (token = strtok_r(result.originalStr, delim, &savePtr); token != NULL;
       token = strtok_r(NULL, delim, &savePtr)) {
    result.token = token;
  }

  // At this point, result.token points to the last token found,
  // and result.originalStr holds the duplicated string that needs to be freed
  // by the caller.
  return result;
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
