//
// Created by main on 28/01/24.
//

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <stdbool.h>

/**
 * @brief Standard struct for HTTP requests.
 */
struct HTTPRequest;

/**
 * @brief Initializes a new HTTP request struct from a string.
 * @param string The string to parse.
 * @return
 */
struct HTTPRequest *initializeHTTPRequestFromString(char *string);

/**
 * @bried Initializes a new HTTP request struct.
 * @param method The method, e.g. GET, POST, HEAD
 * @param path The path, e.g. ./index.html
 * @param protocol The protocol, e.g. 1.0, 1.1
 * @return struct HTTPRequest
 */
struct HTTPRequest *initializeHTTPRequest(const char *method, const char *path, const char *protocol);

/**
 * @brief Strips the given string of return characters.
 * @param string The string to strip.
 * @return
 */
char *stripHTTPRequestReturnCharacters(char *string);

/**
 * @brief Returns true if the given HTTP method is valid.
 * Returns false if it is not.
 * @param method The HTTP request
 * @return true or false
 */
bool isValidHTTPMethod(char *method);

/**
 * @brief Prints the values of an HTTPRequest struct.
 * @param request The struct to print.
 */
void printHTTPRequestStruct(const struct HTTPRequest *request);

#endif    // HTTPREQUEST_H
