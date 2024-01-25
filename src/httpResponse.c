//
// Created by main on 25/01/24.
//

// Contains HTTP response info.
struct httpResponse {
    // 200, 500, ETC.
    int status;

    // Any info.
    char * body;
};