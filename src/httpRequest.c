//
// Created by main on 25/01/24.
//

// Contains HTTP request info.
struct httpRequest {
    // 200, 400, 500, etc.
    int status;

    // GET, POST, etc.
    char * type;

    // File path, e.g. index.html
    char * filePath;

    // HTTP/1.0, etc.
    char * version;
};