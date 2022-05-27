#ifndef PROJECT_HEADER_H
#define PROJECT_HEADER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NOT_FOUND -1
#define TRUE 1
#define FALSE 0
#define DELIMITER " ,.;:?!-\t'()[]{}<>~_\n"
#define MAX_LINE_LENGTH 151
#define HOUR 60
#define MEM_ERROR "MEMORY ALLOCATION ERROR! EXITING PROGRAM!"
#define FILE_ERROR "FILE ERROR! EXITING PROGRAM!"

typedef int BOOL;


char* formatRawStr(char* str);
void checkAllocation(void* ptr);
void checkOpen(FILE* f);

#endif //PROJECT_HEADER_H
