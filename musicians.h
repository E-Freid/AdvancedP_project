#ifndef PROJECT_MUSICIANS_H
#define PROJECT_MUSICIANS_H

#include "header.h"
#include "tree.h"

typedef struct musicianPriceInstrument {
    unsigned short insId;
    float price;
    struct musicianPriceInstrument* next;
} MusicianPriceInstrument;

typedef struct mpiList {
    MusicianPriceInstrument* head;
    MusicianPriceInstrument* tail;
} MPIList;

typedef struct musician {
    char** name;  //Array of name parts (ie: first name, middle name and last name)
    int logSize;
    int phySize;
    MPIList instruments;  // MusicianPriceInstrument instances in linked list
} Musician;

// Musician methods
Musician** getMusiciansListFromFile(char* fileName, int* size, InstrumentTree* tr);
Musician* getMusicianFromStr(char* str, InstrumentTree* tr);
void initializeMusician(Musician* musician);
void setNewNamePart(Musician* musician, char* str);
char* getNamePart(char* src);

// List methods
void makeEmptyList(MPIList* lst);
MusicianPriceInstrument* createNewListNode(unsigned short insId, float price, MusicianPriceInstrument* next);
void insertDataToEndList(MPIList * lst, unsigned short insId, float price);
void insertNodeToEndList(MPIList* lst, MusicianPriceInstrument* newTail);

// Utils
void freePointersMusiciansArr(Musician ** musicians, int size);
void freePointerCharArr(char** arr, int size);
void freeMPIListRec(MusicianPriceInstrument* mpi);

//Debug methods
void printList(MPIList* lst);

#endif //PROJECT_MUSICIANS_H
