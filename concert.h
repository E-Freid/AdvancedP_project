
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
    BOOL isConcertPossible;
    CIList instruments;
} Concert;

char* getLineFromUser();
char* getNameAndDate(Concert* concert, char* line);
BOOL getInstruments(Concert* concert, char* token, InstrumentTree* tree, Musician*** musiColl, Sizes* sizes);
BOOL getMusicinsForInstrument(ConcertInstrument* instrument, Musician** musicianArr, int size, int numOfmusi);
void getConcerts(Musician*** musiColl, Sizes* sizes,Musician** MusiciansGroup, int numOfMusicians, InstrumentTree* tree);
void resetMusiciansStatus(Musician** musicians, int size);
Concert getConcertFromLine(char* line, InstrumentTree* tree, Musician*** musiColl, Sizes* sizes);
float getMusicianInstrumentPrice(Musician* musician, unsigned short instrumentId);
int getMinutes(float hour);

// Printing
void printConcert(Concert* concerts);
void printInstrumentsList(CIList* instrumentsList, float* totalPrice);
void printBookedMusicians(Musician** bookedMusicians, int numOfMusicians, char* instrumentName, int instrumentId, float* totalPrice);
void printMusicianName(char** name, int size);

// Sorting Utils
void sortMusiciansByPrice(Musician** musicians, int size, int instrumentId, int importance);
void merge(Musician** arr1, Musician** arr2, int size1, int size2, int instrumentId, int importance, Musician** dest);
void mergeHandler(Musician** src, Musician** dest, int* srcInd, int* destInd);
void copyArr(Musician** dest, Musician** src, int size);
int comparePrices(Musician* mus1, Musician* mus2, unsigned short instrumentId, int importance);

// List Methods
CIListNode* createNewCiListNode(ConcertInstrument instrument, CIListNode* next);
void addDataConcertList(Concert* concert, ConcertInstrument instrument);
void addCiListNodeToEndList(CIList* lst, CIListNode* newTail);
void makeEmptyConcertList(CIList* lst);

// Free helpers
void freeConcertData(Concert* concerts);
void freeConcertInstrumentsList(CIListNode* head);


#endif //PROJECT_CONCERT_H
