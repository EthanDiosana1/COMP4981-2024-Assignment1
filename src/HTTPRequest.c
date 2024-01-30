//
// Created by main on 28/01/24.
//

#include "HTTPRequest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_HTTP_REQUEST_TOKENS 3
#define RETURN_CHARACTERS "\\r\\n"

struct HTTPRequest
{
    // GET, POST, etc.
    char *method;

    // ./index.html, etc.
    char *path;

    char *protocol;
};

enum HTTPStatusCodes
{
    OK                    = 200,
    BAD_REQUEST           = 400,
    INTERNAL_SERVER_ERROR = 500,
};

bool isValidHTTPMethod(char *method)
{
    if(method)
    {
        if(strcmp(method, "GET") == 0 || strcmp(method, "POST") == 0 || strcmp(method, "HEAD") == 0)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Returns the number of tokens.
 * @param string The string to tokenize.
 * @param delim The delimiter.
 * @return The number of tokens.
 */
int getNumberOfTokens(char *string, const char *delim)
{
    int   count;
    char *strcopy;
    char *token;

    // Duplicate string.
    strcopy = strdup(string);
    if(strcopy == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Tokenize until there are no more tokens left.
    count = 0;
    token = strtok(strcopy, delim);
    while(token != NULL)
    {
        count++;
        token = strtok(NULL, delim);
    }

    free(strcopy);
    return count;
}

char *stripHTTPRequestReturnCharacters(char *string)
{
    char *copy;

    // Copy the string.
    copy = strdup(string);

    // Tokenize based on return characters,
    // set output as first part.
    char *output = strtok(copy, RETURN_CHARACTERS);

    return output;
}

struct HTTPRequest *initializeHTTPRequestFromString(char *string)
{
    const char *delim = " ";
    char       *tokens[NUM_HTTP_REQUEST_TOKENS];
    char       *token;
    int         count;

    // If the string is null, exit with error.
    if(!string)
    {
        fprintf(stderr, "!string\n");
        exit(EXIT_FAILURE);
    }

    // Duplicate the string before tokenization.
    char *stringCopy = strdup(string);
    if(stringCopy == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Tokenize the duplicated string.
    token = strtok(stringCopy, delim);

    // Store tokens in an array.
    count = 0;
    while(token != NULL && count < NUM_HTTP_REQUEST_TOKENS)
    {
        // Place the token.
        tokens[count] = strdup(token);

        // Check for mem fail.
        if(tokens[count] == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        // Move to the next token.
        token = strtok(NULL, delim);
        count++;
    }

    // If not the correct number of tokens, exit with error.
    if(count != NUM_HTTP_REQUEST_TOKENS)
    {
        fprintf(stderr, "Incorrect number of tokens: %d\n", count);
    }

    // Set the variables.
    const char *method   = tokens[0];
    const char *filePath = tokens[1];
    // Strip the return characters from the protocol.
    const char *protocol = stripHTTPRequestReturnCharacters(tokens[2]);

    // Initialize the struct.
    struct HTTPRequest *request = initializeHTTPRequest(method, filePath, protocol);

    // Free duplicated string.
    free(stringCopy);

    // Free the tokens.
    for(int i = 0; i < NUM_HTTP_REQUEST_TOKENS; i++)
    {
        free(tokens[i]);
    }

    return request;
}

struct HTTPRequest *initializeHTTPRequest(const char *method, const char *path, const char *protocol)
{
    // If any of the args are null, exit.
    // This will be handled by errors later.
    if(!method)
    {
        fprintf(stderr, "!method\n");
        exit(EXIT_FAILURE);
    }
    else if(!path)
    {
        fprintf(stderr, "!path\n");
        exit(EXIT_FAILURE);
    }
    else if(!protocol)
    {
        fprintf(stderr, "!protocol\n");
        exit(EXIT_FAILURE);
    }

    // Allocate struct memory.
    struct HTTPRequest *request = (struct HTTPRequest *)malloc(sizeof(struct HTTPRequest));

    // If allocate failed, exit.
    if(!request)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Copy strings.
    request->method   = strdup(method);
    request->path     = strdup(path);
    request->protocol = strdup(protocol);

    return request;
}

void printHTTPRequestStruct(const struct HTTPRequest *request)
{
    printf("struct HTTPRequest: {"
           "\n\tMethod: %s"
           "\n\tPath: %s"
           "\n\tProtocol: %s"
           "\n}\n",
           request->method,
           request->path,
           request->protocol);
}