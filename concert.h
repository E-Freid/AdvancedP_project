
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
Concert** getConcerts(Musician*** musiColl, Sizes* sizes,Musician** MusiciansGroup, int numOfMusicians,
                      InstrumentTree* tree, int* numOfConcerts);
void resetMusiciansStatus(Musician** musicians, int size);
Concert* getConcertFromLine(char* line, InstrumentTree* tree, Musician*** musiColl, Sizes* sizes);
void printConcerts(Concert** concerts, int numOfConcerts);
void printInstrumentsList(CIList* instrumentsList);
void printBookedMusicians(Musician** bookedMusicians, int numOfMusicians, char* instrumentName, int instrumentId);
void printMusicianName(char** name, int size);
float getMusicianInstrumentPrice(Musician* musician, unsigned short instrumentId);
int getMinutes(float hour);

// List Methods
CIListNode* createNewCiListNode(ConcertInstrument instrument, CIListNode* next);
void addDataConcertList(Concert* concert, ConcertInstrument instrument);
void addCiListNodeToEndList(CIList* lst, CIListNode* newTail);
void makeEmptyConcertList(CIList* lst);

// Free helpers
void freeConcerts(Concert** concerts, int size);
void freeConcertInstrumentsList(CIListNode* head);


#endif //PROJECT_CONCERT_H
