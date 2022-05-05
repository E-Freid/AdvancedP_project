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

char** txtToArray(char* filePath, int* size){
    /// this function creates an array of strings from a .txt file.
    /// it returns the array and the size of the array.
    /// time complexity: say n is the number of lines in the file, then O(n)
    int logSize, phySize;
    logSize = 0, phySize=2;
    BOOL reachedEOF = FALSE;

    FILE* f = fopen(filePath, "r");
    checkOpen(f);

    char** strArr = (char**) malloc(sizeof(char*) * phySize);
    checkAllocation(strArr);

    while(!reachedEOF){
        if(logSize == phySize){
            phySize *= 2;
            strArr = (char**) realloc(strArr, sizeof(char*) * phySize);
            checkAllocation(strArr);
        }

        strArr[logSize] = (char*) malloc(sizeof(char) * MAX_LINE_LENGTH);   // get line from file
        fgets(strArr[logSize], MAX_LINE_LENGTH, f);

        if(feof(f)) {                                   // if finished already, free the line and break
            free(strArr[logSize]);
            reachedEOF = TRUE;
        }
        else {                                      // change '\n' to '\0' and reallocate to logical size
            formatRawStr(strArr[logSize]);
            logSize++;
        }
    }

    strArr = (char**) realloc(strArr, sizeof(char*) * logSize);
    checkAllocation(strArr);
    *size = logSize;

    fclose(f);
    return strArr;
}

void formatRawStr(char* str){
    /// this function deletes the '\n' at the end of str, and reallocates the str to its logical size
    /// time complexity: 1 <= strSize <= 150. therefore, O(strSize) = O(1)!
    int strSize;

    strSize = strlen(str);
    str[strSize-1] = '\0';

    str = (char*) realloc(str, sizeof(char) * strSize);
    checkAllocation(str);
}