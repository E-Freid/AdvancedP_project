
#ifndef PROJECT_CONCERT_H
#define PROJECT_CONCERT_H

#include "musicians.h"

typedef struct
{
    int day,month,year;
    float hour;
} Date;

typedef struct {
    int num;
    int inst;
    char* name;
    char importance;
    Musician* bookedMusicians;
} ConcertInstrument;

typedef struct ciListNode{
    ConcertInstrument instrument;
    struct ciListNode* next;
} CIListNode;

typedef struct ciList{
    CIListNode* head;
    CIListNode* tail;
} CIList;

typedef struct{
    Date date_of_concert;
    char* name;
    CIList instruments;
} Concert;

char* getLineFromUser(char startingChar);
void getConcerts(Musician*** musiColl, Sizes* sizes);
void getNameAndDate(Concert* concert, char* line);

#endif //PROJECT_CONCERT_H
