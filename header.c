#include "header.h"

void checkAllocation(void* ptr){
    /// this function checks if a memory allocation was successful
    if(ptr == NULL) {
        printf(MEM_ERROR);
        exit(1);
    }
}

void checkOpen(FILE* f){
    /// this function checks if a file opening was successful
    if(f == NULL) {
        printf(FILE_ERROR);
        exit(1);
    }
}

char* formatRawStr(char* str) {
    /// this function takes a str with unknown length, and shrinks it to the logical size. also removes the '\n'.
    int strSize;
    strSize = strlen(str);
    str[strSize-1] = '\0';

    char* newStr = (char*) malloc(sizeof(char) * strSize);
    checkAllocation(newStr);

    strcpy(newStr, str);

    return newStr;
}