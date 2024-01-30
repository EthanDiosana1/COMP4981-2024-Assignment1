//
// Created by main on 28/01/24.
//

#include "fileTools.h"
#include "stringTools.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct fileData {
    // The number of chars in the file name.
    int fileNameLength;

    // The file name.
    char * fileName;

    // The number of chars in the file.
    long contentLength;

    // The content of the file as a string.
    char * content;
};

struct fileData * initializeFileDataStruct(int fileNameLength, char * fileName, long contentLength, char * content) {
    struct fileData * fileData;

    // If invalid fileNameLength, exit with error.
    if (fileNameLength <= 0) {
        char errorMSG[] = "Invalid fileNameLength: %d\n";
        fprintf(stderr, errorMSG, fileNameLength);
        exit(EXIT_FAILURE);
    }

    // If invalid contentLength, exit with error.
    if (contentLength < 0) {
        char errorMSG[] = "Invalid contentLength: %d\n";
        fprintf(stderr, errorMSG, contentLength);
        exit(EXIT_FAILURE);
    }

    // If !fileName, exit with error.
    if (!fileName) {
        fprintf(stderr, "!fileName\n");
        exit(EXIT_FAILURE);
    }

    // If !content, exit with error.
    if (!content) {
        perror("!content\n");
        exit(EXIT_FAILURE);
    }

    // Allocate the memory.
    fileData = (struct fileData*)malloc(sizeof(struct fileData));

    // If the memory failed to allocate, exit with an error.
    if (!fileData) {
        perror("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Insert the ints.
    fileData->fileNameLength = fileNameLength;
    fileData->contentLength = contentLength;

    // Copy the strings.
    fileData->fileName = strdup(fileName);
    fileData->content = strdup(content);

    return fileData;
}

struct fileData * getFileDataFromFilePath(char * filePath) {
    struct fileData * fileData;
    FILE * file;

    // File data params.
    int fileNameLength;
    long contentLength;
    char * fileName;
    char * content;

    // If !filePath, exit with error.
    if (!filePath) {
        perror("!filePath\n");
        exit(EXIT_FAILURE);
    }

    // Open the file.
    file = fopen(filePath, "r");

    // If the file does not open, exit with error.
     if (!file) {
         fprintf(stderr, "!file: Error opening file.\n");
         exit(EXIT_FAILURE);
     }

    // Get the file size.
    fseek(file, 0, SEEK_END);
     contentLength = ftell(file);
     fseek(file, 0, SEEK_SET);

     // Allocate memory for the content.
     content = (char*)malloc(contentLength+1);

     // If content failed to allocate, exit with error.
     if (!content) {
         perror("Memory allocation failed.");
         exit(EXIT_FAILURE);
     }

     // Read and store the content.
     fread(content, 1, contentLength, file);
     content[contentLength] = '\0';

     // Get the file name.
     fileName = getLastToken(filePath, "/");
     fileNameLength = (int)strlen(fileName);

     fileData = initializeFileDataStruct(
             fileNameLength,
             fileName,
             contentLength,
             content
             );

    return fileData;
}

void printFileDataStruct(const struct fileData * fileData) {
    printf(
            "struct fileData: {"
            "\n\tfileNameLength: %d"
            "\n\tfileName: %s"
            "\n\tcontentLength: %ld"
            "\n\tcontent: %s\n}",
            fileData->fileNameLength,
            fileData->fileName,
            fileData->contentLength,
            fileData->content
            );
}