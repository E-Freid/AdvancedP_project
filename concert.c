
#include "concert.h"

void getConcerts(Musician*** musiColl, Sizes* sizes, InstrumentTree* tree){
    char ch;
    char* line, *token;
    Concert concert;
    BOOL concertIsPossible;
    makeEmptyConcertList(&concert.instruments);

    ch = getchar();

    while(ch != '\n') {
        line = getLineFromUser(ch);
        token = getNameAndDate(&concert,line);
        concertIsPossible = getInstruments(&concert,token,tree,musiColl,sizes);
        free(line);
        /// print function for the concert with concertIsPossible bool to know what to print
        /// reset function for the musicians that were taken for the concert (maybe do it inside of freeConcert instead)
        freeConcert(concert);
        ch = getchar(); // advance to the next concert
    }
}

char* getLineFromUser(char startingChar){
    char ch = startingChar;
    char* line;

    int logSize,phySize;
    logSize = 0, phySize = 2;
    line = (char*) malloc(sizeof(char) * phySize);
    checkAllocation(line);

    while(ch != '\n'){
        if(logSize == phySize){
            phySize *= 2;
            line = (char*) realloc(line, sizeof(char) * phySize);
            checkAllocation(line);
        }
        line[logSize] = ch;     // get char into the str
        logSize++;
        ch = getchar();
    }
    line = realloc(line, sizeof(char) * logSize + 1);
    line[logSize] = '\0';

    return line;
}

char* getNameAndDate(Concert* concert, char* line){
    int nameLen,i;
    char deli[] = " ";
    char* token;
    float hourAddition;

    token = strtok(line, deli);

    nameLen = strlen(token);    // get the name
    concert->name = (char*) malloc(sizeof(char) * nameLen + 1);
    checkAllocation(concert->name);
    strcpy(concert->name, token);

    for(i=0;i<4;i++){           // get the dates
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
        if(musicianArr[i]->isTakenAlready == FALSE){
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

void freeConcert(Concert concert){
    free(concert.name);
    freeConcertInstrumentsList(concert.instruments.head);
}

void freeConcertInstrumentsList(CIListNode* head){
    if(head == NULL)
        return;
    freeConcertInstrumentsList(head->next);
    free(head->instrument.name);
    if(head->instrument.bookedMusicians != NULL)
        free(head->instrument.bookedMusicians);
}

void makeEmptyConcertList(CIList* lst){
    lst->head = lst->tail = NULL;
}