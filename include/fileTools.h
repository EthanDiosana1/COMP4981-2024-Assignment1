//
// Created by main on 28/01/24.
//

#ifndef FILETOOLS_H
#define FILETOOLS_H

#include <stdbool.h>

/**
 * Contains the data of a file,
 * including name, contents, and content length.
 */
struct fileData;

/**
 * @brief Returns a new fileData struct from a file path.
 * @param filePath The path to the file.
 * @return struct * fileData
 */
struct fileData *getFileDataFromFilePath(char *filePath);

/**
 * @brief Returns a new fileData struct from the params.
 * @param fileNameLength The length of the file name.
 * @param fileName The file name.
 * @param contentLength The length of the file content.
 * @param content The file content.
 * @return fileData struct
 */
struct fileData *initializeFileDataStruct(int fileNameLength, char *fileName,
                                          long contentLength, char *content);

/**
 * @brief Prints a fileData struct to the console.
 * @param fileData The struct to print.
 */
void printFileDataStruct(const struct fileData *fileData);

#endif // FILETOOLS_H
