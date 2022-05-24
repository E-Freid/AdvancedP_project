
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
        //printConcert(concert, concertIsPossible);
        freeConcert(concert);
        ch = getchar();
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
        line[logSize] = ch;
        logSize++;
        ch = getchar();
    }

    return line;
}

char* getNameAndDate(Concert* concert, char* line){
    int nameLen,i;
    Date date;
    char deli[] = " ";
    char* token;
    float hourAddition;

    sscanf(line, "%d %d %d %f:%f", &date.day, &date.month, &date.year, &date.hour, &hourAddition);  // get dates before
    date.hour += (hourAddition) / HOUR;                                                              // changing the line

    token = strtok(line, deli);

    nameLen = strlen(token);
    concert->name = (char*) malloc(sizeof(char) * nameLen + 1);
    strcpy(concert->name, token);

    for(i=0;i<4;i++){
        token = strtok(NULL, deli);     // advance to the instruments
    }
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

        // sort here musicollections[instrument.inst] according to instrument.importance

        concertIsPossible = getMusicinsForInstrument(&instrument,musiColl[instrument.inst],
                                                     sizes[instrument.inst].phySize, instrument.num);

        addDataConcertList(concert,instrument);
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
    node->instrument.name = instrument.name;
    node->instrument.bookedMusicians = instrument.bookedMusicians;
    node->instrument.inst = instrument.inst;
    node->instrument.num = instrument.num;
    node->instrument.importance = instrument.importance;
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
    free(head->instrument.bookedMusicians);
}