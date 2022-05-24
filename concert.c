
#include "concert.h"

void getConcerts(Musician*** musiColl, Sizes* sizes, InstrumentTree* tree, ){
    char ch;
    char* line, *token;
    Concert concert;

    ch = getchar();

    while(ch != '\n') {
        line = getLineFromUser(ch);
        token = getNameAndDate(&concert,line);
        token = getInstruments(&concert,token, tree);
        free(line);
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

char* getInstruments(Concert* concert, char* token, InstrumentTree* tree, Musician*** musiColl, Sizes* sizes){
    ConcertInstrument instrument;
    int instrumentNameLen;
    char deli[] = " ";

    while(token != NULL){

        instrumentNameLen = strlen(token);          // get instruments name
        instrument.name = (char*) malloc(sizeof(char) * instrumentNameLen + 1);
        strcpy(instrument.name, token);

        instrument.inst = findInsId(tree, instrument.name); // get instrument ID

        token = strtok(NULL, deli);             // get num of players on specific instrument
        sscanf(token, "%d", &instrument.num);

        token = strtok(NULL, deli);         // get importance of instrument
        sscanf(token, "%c", &instrument.importance);

        // sort here musicollections[instrument.inst] according to instrument.importance

        getMusicinsForInstrument(&instrument, musiColl[instrument.inst], sizes[instrument.inst].phySize);
        addDataConcertList(concert,instrument);
    }
}