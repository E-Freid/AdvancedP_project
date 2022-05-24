
#include "concert.h"

void getConcerts(Musician*** musiColl, Sizes* sizes){
    char ch;
    char* line;
    Concert concert;


    ch = getchar();
    while(ch != '\n') {
        line = getLineFromUser(ch);
        getNameAndDate(&concert,line);
        getInstruments(&concert, line);
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

void getNameAndDate(Concert* concert, char* line){
    int nameLen,i;
    Date date;
    char deli[] = " ";
    char* token;
    float hourAddition;

    sscanf(line, "%d %d %d %f:%f", &date.day, &date.month, &date.year, &date.hour, &hourAddition);  // get dates before
    date.hour += (hourAddition) / 60;                                                              // changing the line

    token = strtok(line, deli);

    nameLen = strlen(token);
    concert->name = (char*) malloc(sizeof(char) * nameLen + 1);
    strcpy(concert->name, token);

    for(i=0;i<4;i++){
        token = strtok(NULL, deli);     // advance to the instruments
    }
}