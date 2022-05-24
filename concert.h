
#ifndef PROJECT_CONCERT_H
#define PROJECT_CONCERT_H

#include "musicians.h"
#include "tree.h"

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
    Musician** bookedMusicians;
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
char* getNameAndDate(Concert* concert, char* line);
CIListNode* createNewCiListNode(ConcertInstrument instrument, CIListNode* next);
BOOL getInstruments(Concert* concert, char* token, InstrumentTree* tree, Musician*** musiColl, Sizes* sizes);
BOOL getMusicinsForInstrument(ConcertInstrument* instrument, Musician** musicianArr, int size, int numOfmusi);
void getConcerts(Musician*** musiColl, Sizes* sizes, InstrumentTree* tree);
void addDataConcertList(Concert* concert, ConcertInstrument instrument);
void addCiListNodeToEndList(CIList* lst, CIListNode* newTail);
void makeEmptyConcertList(CIList* lst);
void freeConcert(Concert concert);
void freeConcertInstrumentsList(CIListNode* head);


#endif //PROJECT_CONCERT_H
