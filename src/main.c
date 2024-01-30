#include <stdio.h>
#include "HTTPRequest.h"
#include "fileTools.h"
#include <stdlib.h>

void testHTTPRequest() {
    char testReq[] = "GET ./index.html HTTP/1.1\\r\\n";

    printf("Request: %s\n", testReq);

    struct HTTPRequest * req = initializeHTTPRequestFromString(testReq);

    printHTTPRequestStruct(req);
}

void testFileTools() {
    const char filePath[] = "../html/index.html";

    struct fileData * fileData = getFileDataFromFilePath(filePath);

    printFileDataStruct(fileData);
}

int main() {

    testHTTPRequest();

    testFileTools();

    return 0;
}