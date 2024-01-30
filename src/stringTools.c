//
// Created by main on 28/01/24.
//
#include <stdlib.h>
#include <string.h>

char *getLastToken(const char *string, const char *delim)
{
    char *strcopy;
    char *token;
    char *lastToken;

    // Duplicate the string.
    strcopy = strdup(string);

    // Tokenize the string.
    token = strtok((char *)strcopy, delim);

    // Get the last token.
    while(token)
    {
        lastToken = token;
        token     = strtok(NULL, delim);
    }

    // Free.
    free(strcopy);
    free(token);

    return lastToken;
}

int getNumberOfTokens(const char *string, const char *delim)
{
    int   count;
    char *strcopy;
    char *token;

    // Duplicate the string.
    strcopy = strdup(string);

    // Initialize count to 0;
    token = strtok(strcopy, delim);

    // Count until no more tokens.
    while(token)
    {
        count++;
        token = strtok(NULL, delim);
    }

    // Free duplicated string.
    free(strcopy);

    return count;
}

char *strTokToArray(const char *string, const char *delim)
{
    char  *strcopy;
    char  *token;
    char **tokenArray;
    int    numTokens;
    int    count;

    // Get the number of tokens.
    numTokens = getNumberOfTokens(string, delim);

    // Allocate the memory for the token array.
    tokenArray = (char **)malloc(sizeof(char *) * numTokens);

    // Tokenize and store in the array.

    // Free.
    free(strcopy);
    free(token);

    return tokenArray;
}