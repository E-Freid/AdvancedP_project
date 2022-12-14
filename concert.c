#include "concert.h"

void getConcerts(Musician*** musiColl, Sizes* sizes,Musician** MusiciansGroup, int numOfMusicians, InstrumentTree* tree) {
    /// this function gets concert information from the user and then tries to gather musicians
    char* line;
    Concert concert;

    while ((line = getLineFromUser()) != NULL) {        // while not finished
        concert = getConcertFromLine(line, tree, musiColl, sizes);
        printConcert(&concert);
        resetMusiciansStatus(MusiciansGroup, numOfMusicians);
        freeConcertData(&concert);
    }
}

Concert getConcertFromLine(char* line, InstrumentTree* tree, Musician*** musiColl, Sizes* sizes) {
    /// this function gets the information about the concert from the user and returns a Concert struct
    char* token;
    Concert concert;

    makeEmptyConcertList(&concert.instruments);
    token = getNameAndDate(&concert, line);
    concert.isConcertPossible = getInstruments(&concert, token, tree, musiColl, sizes);
    free(line);

    return concert;
}

char* getLineFromUser(){
    /// this function takes input from the user and returns a str
    int ch;
    char* line;

    int logSize,phySize;
    logSize = 0, phySize = 2;
    line = (char*) malloc(sizeof(char) * phySize);
    checkAllocation(line);

    while((ch = getchar()) != '\n' && ch != EOF){ // while not finished reading the line
        if(logSize == phySize){
            phySize *= 2;
            line = (char*) realloc(line, sizeof(char) * phySize);
            checkAllocation(line);
        }
        line[logSize] = ch;     // get char into the str
        logSize++;
    }
    if(logSize == 0){   // empty line
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
    /// this function takes the str and gets the name and date information
    int nameLen,i;
    char deli[] = " ";
    char* token;
    float hourAddition;

    token = strtok(line, deli);

    nameLen = strlen(token);    // get the name
    concert->name = (char*) malloc(sizeof(char) * (nameLen + 1));
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
    /// this function gets the instruments information from the str and tries to add musicians to the concert
    /// it returns true if found enough musicians for all the instruments. else, false.
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

        if(instrument.inst == NOT_FOUND){   // instrument does not exist
            concertIsPossible = FALSE;
            instrument.bookedMusicians = NULL;
        }

        else {      // instrument exists, find musicians

            sortMusiciansByPrice(musiColl[instrument.inst], sizes[instrument.inst].phySize, instrument.inst, instrument.importance);

            concertIsPossible = getMusicinsForInstrument(&instrument, musiColl[instrument.inst],
                                                                  sizes[instrument.inst].phySize, instrument.num);

        }

        addDataConcertList(concert,instrument);

        token = strtok(NULL, deli); // advance to next instrument
    }
    return concertIsPossible;
}

BOOL getMusicinsForInstrument(ConcertInstrument* instrument, Musician** musicianArr, int size, int numOfmusi){
    /// this function gathers musicians to play on the instrument and returns True if the process was successful.
    /// else, false.
    int i;
    int bookedArrInd;
    instrument->bookedMusicians = (Musician**) malloc(sizeof(Musician*) * numOfmusi);
    checkAllocation(instrument->bookedMusicians);
    for(i=bookedArrInd=0;i<size && bookedArrInd<numOfmusi;i++){
        if(musicianArr[i]->isTakenAlready == FALSE) {
            instrument->bookedMusicians[bookedArrInd] = musicianArr[i]; // add musician and change status to taken.
            musicianArr[i]->isTakenAlready = TRUE;
            bookedArrInd++;
        }
    }

    if(bookedArrInd < numOfmusi){  // not enough musicians for the instrument
        return FALSE;
    }

    return TRUE;
}

void printConcert(Concert* concert) {
    /// this function prints the concert information
    Date* date = &concert->date_of_concert;
    float totalPrice = 0;

    if(!concert->isConcertPossible)
        printf("Could not find musicians for the concert %s\n", concert->name);

    else {
        printf("%s ", concert->name);
        printf("%02d %02d %04d %02d:%02d: ", date->day, date->month, date->year,
               (int)date->hour, getMinutes(date->hour));
        printInstrumentsList(&concert->instruments, &totalPrice);
        printf(" Total Cost: %d.\n", (int)totalPrice);
    }
}

void printInstrumentsList(CIList* instrumentsList, float* totalPrice) {
    /// this function prints the musicians and instruments information
    CIListNode* cur = instrumentsList->head;
    ConcertInstrument* curInstrument;

    while(cur != NULL) {
        curInstrument = &cur->instrument;
        printBookedMusicians(curInstrument->bookedMusicians, curInstrument->num,
                                curInstrument->name, curInstrument->inst, totalPrice);
        if(cur->next == NULL)
            printf(". ");
        else
            printf(", ");
        cur = cur->next;
    }
}

void printBookedMusicians(Musician** bookedMusicians, int numOfMusicians, char* instrumentName, int instrumentId, float* totalPrice) {
    /// this function prints the musicians playing on a specific instrument and their prices
    float price;

    for (int i = 0; i < numOfMusicians; ++i) {
        price = getMusicianInstrumentPrice(bookedMusicians[i], instrumentId);
        *totalPrice += price;
        printMusicianName(bookedMusicians[i]->name, bookedMusicians[i]->logSize);
        printf("- %s (%d)", instrumentName, (int)price);
        if(i != (numOfMusicians - 1))
            printf(", ");
    }
}

void printMusicianName(char** name, int size) {
    /// this function prints the musician full name
    for (int i = 0; i < size; ++i) {
        printf("%s ", name[i]);
    }
}

float getMusicianInstrumentPrice(Musician* musician, unsigned short instrumentId) {
    /// this function returns the price the musician takes for playing the instrument
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
    /// this function calculates and returns the minutes
    int intPart = (int)hour;
    float decimalPart = hour - (float)intPart;

    return decimalPart * HOUR;
}

void resetMusiciansStatus(Musician** musicians, int size) {
    /// Function that resets the isTakenAlready flag for each musician to false before getting new concert data
    for (int i = 0; i < size; ++i) {
        musicians[i]->isTakenAlready = FALSE;
    }
}

// Sort Utils
void sortMusiciansByPrice(Musician** musicians, int size, int instrumentId, int importance) { // Merge sort implementation
    /// this function sorts the musicians ptr arrays according to the price and importance
    if(size <= 1)
        return;
    else {
        int m = size / 2;

        sortMusiciansByPrice(musicians, m, instrumentId, importance);
        sortMusiciansByPrice(musicians + m, size - m, instrumentId, importance);

        Musician** tmpArr = (Musician**)malloc(sizeof(Musician*) * size);
        checkAllocation(tmpArr);

        merge(musicians, musicians + m, m, size - m, instrumentId, importance, tmpArr);
        copyArr(musicians, tmpArr, size); // copy values from tmpArr to arr
        free(tmpArr);
    }
}

void merge(Musician** arr1, Musician** arr2, int size1, int size2, int instrumentId, int importance, Musician** dest) {
    /// this function merges 2 arrays of musicians ptr into 1 array according to the prices and importance
    int i1 = 0, i2 = 0, iRes = 0;

    while(i1 < size1 && i2 < size2) {
        if(comparePrices(arr1[i1], arr2[i2], instrumentId, importance) > 0) {
            mergeHandler(arr1, dest, &i1, &iRes);
        } else {
            mergeHandler(arr2, dest, &i2, &iRes);
        }
    }
    while (i1 < size1) {
        mergeHandler(arr1, dest, &i1, &iRes);
    }
    while (i2 < size2) {
        mergeHandler(arr2, dest, &i2, &iRes);
    }
}

void mergeHandler(Musician** src, Musician** dest, int* srcInd, int* destInd) {
    /// this function copies a musician ptr from src to dest. according to the indexes.
    int i = *srcInd;
    int iRes = *destInd;

    dest[iRes] = src[i];
    *srcInd = ++i;
    *destInd = ++iRes;
}

void copyArr(Musician** dest, Musician** src, int size) {
    /// this function copies src into dest
    int i;

    for (i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

int comparePrices(Musician* mus1, Musician* mus2, unsigned short instrumentId, int importance) {
    /// this function compares prices according to the importance and returns a positive or a negative number accordingly.
   float price1, price2, res;

   price1 = getMusicianInstrumentPrice(mus1, instrumentId);
   price2 = getMusicianInstrumentPrice(mus2, instrumentId);

   res = price1 - price2;

   if(res > 0) {
       return importance == '1' ? 1 : -1;
   }

   else
       return importance == '1' ? -1 : 1;
}

// List Methods
void addDataConcertList(Concert* concert, ConcertInstrument instrument){
    /// this function adds data to the end of the instruments list in Concert
    CIListNode* node = createNewCiListNode(instrument, NULL);
    addCiListNodeToEndList(&concert->instruments, node);
}

CIListNode* createNewCiListNode(ConcertInstrument instrument, CIListNode* next){
    /// this function creates and returns a new concert instrument list node
    CIListNode* node = (CIListNode*) malloc(sizeof(CIListNode));
    checkAllocation(node);
    node->instrument = instrument;
    node->next = next;
    return node;
}

void addCiListNodeToEndList(CIList* lst, CIListNode* newTail){
    /// this function adds a new list node to the end of the list
    if(lst->head == NULL)
        lst->head = lst->tail = newTail;
    else{
        lst->tail->next = newTail;
        lst->tail = newTail;
    }
}

void makeEmptyConcertList(CIList* lst){
    /// this function makes the list an empty list
    lst->head = lst->tail = NULL;
}

// Free helpers
void freeConcertData(Concert* concert) {
    /// this function frees the data inside of the concert.
    free(concert->name);
    freeConcertInstrumentsList(concert->instruments.head);
}

void freeConcertInstrumentsList(CIListNode* head){
    /// this function frees the concert instrument list
    if(head == NULL)
        return;
    freeConcertInstrumentsList(head->next);
    free(head->instrument.name);
    if(head->instrument.bookedMusicians != NULL)
        free(head->instrument.bookedMusicians);
    free(head);
}