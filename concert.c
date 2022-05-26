#include "concert.h"

Concert** getConcerts(Musician*** musiColl, Sizes* sizes,Musician** MusiciansGroup, int numOfMusicians,
                      InstrumentTree* tree, int* numOfConcerts) {
    char* line;
    int concertsSize = 0, phySize = 2;

    Concert** concerts = (Concert**)malloc(sizeof(Concert*) * phySize);
    checkAllocation(concerts);

//    BOOL finishedConcerts = FALSE;

    while ((line = getLineFromUser()) != NULL) {
        if(concertsSize == phySize){
            phySize *= 2;
            concerts = (Concert**) realloc(line, sizeof(Concert*) * phySize);
            checkAllocation(concerts);
        }

        concerts[concertsSize] = getConcertFromLine(line, tree, musiColl, sizes);
        concertsSize++;
        resetMusiciansStatus(MusiciansGroup, numOfMusicians);
    }

    if(concertsSize == 0){
        free(concerts);
        concerts = NULL;
    }
    else {
        concerts = (Concert**)realloc(concerts, sizeof(Concert*) * concertsSize);
        checkAllocation(concerts);
    }

    *numOfConcerts = concertsSize;
    return concerts;
}

Concert* getConcertFromLine(char* line, InstrumentTree* tree, Musician*** musiColl, Sizes* sizes) {
    char* token;
    Concert* concert = (Concert*)malloc(sizeof(Concert));
    checkAllocation(concert);

    makeEmptyConcertList(&concert->instruments);
    token = getNameAndDate(concert, line);
    concert->isConcertPossible = getInstruments(concert, token, tree, musiColl, sizes);
    free(line);

    return concert;
}

char* getLineFromUser(){
    int ch;
    char* line;

    int logSize,phySize;
    logSize = 0, phySize = 2;
    line = (char*) malloc(sizeof(char) * phySize);
    checkAllocation(line);

    while((ch = getchar()) != '\n' && ch != EOF){
        if(logSize == phySize){
            phySize *= 2;
            line = (char*) realloc(line, sizeof(char) * phySize);
            checkAllocation(line);
        }
        line[logSize] = ch;     // get char into the str
        logSize++;
    }
    if(logSize == 0){
        free(line);
        line = NULL;
    }
    else {
        line = (char*)realloc(line, sizeof(char) * logSize + 1);
        checkAllocation(line);
        line[logSize] = '\0';
    }

    return line;
}

char* getNameAndDate(Concert* concert, char* line) {
    int nameLen,i;
    char deli[] = " ";
    char* token;
    float hourAddition;

    token = strtok(line, deli);

    nameLen = strlen(token);    // get the name
    concert->name = (char*) malloc(sizeof(char) * nameLen + 1);
    checkAllocation(concert->name);
    strcpy(concert->name, token);

    for(i = 0; i < 4; i++){           // get the dates
        token = strtok(NULL, deli);
        switch(i){
            case 0:
                sscanf(token, "%d", &concert->date_of_concert.day);
                break;
            case 1:
                sscanf(token, "%d", &concert->date_of_concert.month);
                break;
            case 2:
                sscanf(token, "%d", &concert->date_of_concert.year);
                break;
            case 3:
                sscanf(token, "%f:%f", &concert->date_of_concert.hour, &hourAddition);
                concert->date_of_concert.hour += hourAddition/HOUR;
                break;
        }
    }

    token = strtok(NULL, deli); // advance to the instruments
    return token;
}

BOOL getInstruments(Concert* concert, char* token, InstrumentTree* tree, Musician*** musiColl, Sizes* sizes){
    ConcertInstrument instrument;
    int instrumentNameLen;
    char deli[] = " ";
    BOOL concertIsPossible = TRUE;

    while(token != NULL && concertIsPossible){

        instrumentNameLen = strlen(token);          // get instruments name
        instrument.name = (char*) malloc(sizeof(char) * instrumentNameLen + 1);
        checkAllocation(instrument.name);
        strcpy(instrument.name, token);

        instrument.inst = findInsId(tree, instrument.name); // get instrument ID

        token = strtok(NULL, deli);             // get num of players on specific instrument
        sscanf(token, "%d", &instrument.num);

        token = strtok(NULL, deli);         // get importance of instrument
        sscanf(token, "%c", &instrument.importance);

        /// sort here musicollections[instrument.inst] according to instrument.importance

        if(instrument.inst != NOT_FOUND) {
            concertIsPossible = getMusicinsForInstrument(&instrument, musiColl[instrument.inst],
                                                         sizes[instrument.inst].phySize, instrument.num);
        }
        else
            instrument.bookedMusicians = NULL;

        addDataConcertList(concert,instrument);

        token = strtok(NULL, deli); // advance to next instrument
    }
    return concertIsPossible;
}

BOOL getMusicinsForInstrument(ConcertInstrument* instrument, Musician** musicianArr, int size, int numOfmusi){
    int i;
    int bookedArrInd;
    instrument->bookedMusicians = (Musician**) malloc(sizeof(Musician*) * numOfmusi);
    checkAllocation(instrument->bookedMusicians);
    for(i=bookedArrInd=0;i<size && bookedArrInd<numOfmusi;i++){
        if(musicianArr[i]->isTakenAlready == FALSE) {
            instrument->bookedMusicians[bookedArrInd] = musicianArr[i];
            musicianArr[i]->isTakenAlready = TRUE;
            bookedArrInd++;
        }
    }

    if(bookedArrInd < numOfmusi){
        return FALSE;
    }

    return TRUE;
}

/* Function that resets the isTakenAlready flag for each musician to false before getting new concert data */
void resetMusiciansStatus(Musician** musicians, int size) {
    for (int i = 0; i < size; ++i) {
        musicians[i]->isTakenAlready = FALSE;
    }
}

void printConcerts(Concert** concerts, int numOfConcerts) {
    Concert* curConcert;
    Date* date;

    printf("Print Concerts info: \n");

    for (int i = 0; i < numOfConcerts; ++i) {
        curConcert = concerts[i];
        date = &curConcert->date_of_concert;

        if(!curConcert->isConcertPossible)
            printf("Could not find musicians for the concert %s\n", curConcert->name);

        else {
            printf("Concert name: %s\n", curConcert->name);
            printf("Concert time: %02d %02d %04d %02d:%02d \n", date->day, date->month, date->year,
                   (int)date->hour, getMinutes(date->hour));
            printInstrumentsList(&curConcert->instruments);
        }
    }
}

void printInstrumentsList(CIList* instrumentsList) {
    CIListNode* cur = instrumentsList->head;
    ConcertInstrument* curInstrument;
    printf("Musicians list for the concert: \n");
    while(cur != NULL) {
        curInstrument = &cur->instrument;
        printBookedMusicians(curInstrument->bookedMusicians, curInstrument->num, curInstrument->name, curInstrument->inst);
        cur = cur->next;
    }
}

void printBookedMusicians(Musician** bookedMusicians, int numOfMusicians, char* instrumentName, int instrumentId) {
    float price;

    for (int i = 0; i < numOfMusicians; ++i) {
        price = getMusicianInstrumentPrice(bookedMusicians[i], instrumentId);
        printMusicianName(bookedMusicians[i]->name, bookedMusicians[i]->logSize);
        printf("%s %f\n", instrumentName, price);
    }
}

void printMusicianName(char** name, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%s ", name[i]);
    }
}

float getMusicianInstrumentPrice(Musician* musician, unsigned short instrumentId) {
    BOOL isFound = FALSE;
    float price = 0;
    MusicianPriceInstrument* cur = musician->instruments.head;

    while(cur != NULL && !isFound) {
        if(cur->insId == instrumentId) {
            isFound = TRUE;
            price = cur->price;
        }
        cur = cur->next;
    }

    return price;
}

int getMinutes(float hour) {
    int intPart = (int)hour;
    float decimalPart = hour - (float)intPart;

    return decimalPart * 60;
}

// List Methods
void addDataConcertList(Concert* concert, ConcertInstrument instrument){
    CIListNode* node = createNewCiListNode(instrument, NULL);
    addCiListNodeToEndList(&concert->instruments, node);
}

CIListNode* createNewCiListNode(ConcertInstrument instrument, CIListNode* next){
    CIListNode* node = (CIListNode*) malloc(sizeof(CIListNode));
    checkAllocation(node);
    node->instrument = instrument;
    node->next = next;
    return node;
}

void addCiListNodeToEndList(CIList* lst, CIListNode* newTail){
    if(lst->head == NULL)
        lst->head = lst->tail = newTail;
    else{
        lst->tail->next = newTail;
        lst->tail = newTail;
    }
}

void makeEmptyConcertList(CIList* lst){
    lst->head = lst->tail = NULL;
}

// Free helpers
void freeConcerts(Concert** concerts, int size) {
    for (int i = 0; i < size; ++i) {
        free(concerts[i]->name);
        freeConcertInstrumentsList(concerts[i]->instruments.head);
    }
    free(concerts);
}

void freeConcertInstrumentsList(CIListNode* head){
    if(head == NULL)
        return;
    freeConcertInstrumentsList(head->next);
    free(head->instrument.name);
    if(head->instrument.bookedMusicians != NULL)
        free(head->instrument.bookedMusicians);
    free(head);
}