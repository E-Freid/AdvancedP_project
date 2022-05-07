
#include "header.h"

void checkAllocation(void* ptr){
    if(ptr == NULL) {
        printf(MEM_ERROR);
        exit(1);
    }
}

void checkOpen(FILE* f){
    if(f == NULL) {
        printf(FILE_ERROR);
        exit(1);
    }
}

char* formatRawStr(char* str){
    int strSize;
    strSize = strlen(str);
    char* newStr = (char*) malloc(sizeof(char) * strSize);
    checkAllocation(newStr);
    copyUntilEnter(newStr, str);
    newStr[strSize-1] = '\0';
    return newStr;
}

void copyUntilEnter(char* dest, char* src){
    int i=0;
    while(src[i] != '\n'){
        dest[i] = src[i];
        i++;
    }
}