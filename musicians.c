#include "musicians.h"

Musician** getMusiciansListFromFile(char* fileName, int* size, InstrumentTree* tr) {
    int logSize = 0, phySize = 2;
    char currLine[MAX_LINE_LENGTH];
    BOOL reachedEOF = FALSE;

    Musician** musiciansArr = (Musician**)malloc(sizeof(Musician*) * phySize);
    checkAllocation(musiciansArr);

    FILE* f = fopen(fileName, "r");
    checkOpen(f);

    while(!reachedEOF) {
        fgets(currLine, MAX_LINE_LENGTH, f);

        if(feof(f))
            reachedEOF = TRUE;   // finished reading the file, exit the loop

        if(phySize == logSize) {
            phySize *= 2;
            musiciansArr = (Musician**)realloc(musiciansArr, sizeof(Musician*) * phySize);
            checkAllocation(musiciansArr);
        }

        musiciansArr[logSize] = getMusicianFromStr(currLine, tr);
        logSize++;
    }

    musiciansArr = (Musician**)realloc(musiciansArr, sizeof(Musician*) * logSize);
    checkAllocation(musiciansArr);
    *size = logSize;

    fclose(f);
    return musiciansArr;
}

Musician* getMusicianFromStr(char* str, InstrumentTree* tr) {
    char *token;
    int instrumentId = NOT_FOUND;
    float curPrice;

    Musician* musician = (Musician*)malloc(sizeof(Musician));
    checkAllocation(musician);

    initializeMusician(musician);

    token = strtok(str, DELIMITER); // Get first token

    while(token != NULL) {
        instrumentId = findInsId(tr, token);

        if(instrumentId == NOT_FOUND) {
            setNewNamePart(musician, token);
        } else {
            //We found an instrument, Let's add it to the list.
            token = strtok(NULL, DELIMITER);  // Get price as string
            sscanf(token, "%f", &curPrice);
            insertDataToEndList(&musician->instruments, instrumentId, curPrice);

            instrumentId = NOT_FOUND; //Reset the flag for next iteration
        }

        token = strtok(NULL, DELIMITER);
    }

    musician->name = (char**)realloc(musician->name, sizeof(char*) * musician->logSize);
    checkAllocation(musician->name);
    musician->phySize = musician->logSize;

    // For Debugging
    printList(&musician->instruments);
    return musician;
}

void initializeMusician(Musician* musician) {
    musician->logSize = 0;
    musician->phySize = 2;

    musician->name = (char**)calloc(musician->phySize, sizeof(char*));
    checkAllocation(musician->name);

    makeEmptyList(&musician->instruments);
}

void setNewNamePart(Musician* musician, char* str) {
    if(musician->phySize == musician->logSize) {
        musician->phySize *= 2;
        musician->name = (char**)realloc(musician->name, sizeof(char*) * musician->phySize);
        checkAllocation(musician->name);
    }
    musician->name[musician->logSize] = getNamePart(str);
    (musician->logSize)++;
}

char* getNamePart(char* src) {
    char* str = (char*)malloc(sizeof(char) * strlen(src) + 1);
    checkAllocation(str);

    strcpy(str, src);
    return str;
}

// List methods
void makeEmptyList(MPIList* lst) {
    lst->head = lst->tail = NULL;
}

BOOL isEmptyList(MPIList* lst) {
    return (lst->head == NULL && lst->tail == NULL) ? TRUE : FALSE;
}

MusicianPriceInstrument* createNewListNode(unsigned short insId, float price, MusicianPriceInstrument* next) {
    MusicianPriceInstrument* node = (MusicianPriceInstrument*)malloc(sizeof(MusicianPriceInstrument));
    checkAllocation(node);

    node->insId = insId;
    node->price = price;
    node->next = next;

    return node;
}

void insertDataToEndList(MPIList* lst, unsigned short insId, float price) {
    MusicianPriceInstrument* newTail = createNewListNode(insId, price, NULL);
    insertNodeToEndList(lst, newTail);
}

void insertNodeToEndList(MPIList* lst, MusicianPriceInstrument* newTail) {
    if(isEmptyList(lst)) {
        lst->tail = lst->head = newTail;
    }
    else {
        lst->tail->next = newTail;
        lst->tail = newTail;
    }

}

// Utils
void freePointersMusiciansArr(Musician ** musicians, int size) {
    int i;

    for (i = 0; i < size; ++i) {
        freePointerCharArr(musicians[i]->name, musicians[i]->logSize);
        freeMPIListRec(musicians[i]->instruments.head);
        free(musicians[i]);
    }

    free(musicians);
}

void freePointerCharArr(char** arr, int size) {
    int i;

    for (i = 0; i < size; ++i) {
        free(arr[i]);
    }

    free(arr);
}

void freeMPIListRec(MusicianPriceInstrument* head) {
    if(head == NULL)
        return;
    else {
        freeMPIListRec(head->next);
        free(head);
    }
}

//For Debug purposes
void printList(MPIList* lst) {
    MusicianPriceInstrument* cur = lst->head;
    while(cur != NULL) {
        printf("ID: %d, price: %f\n", cur->insId, cur->price);
        cur = cur->next;
    }
    printf("end of musician. \n" );
}
