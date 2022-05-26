#include "tree.h"
#include "musicians.h"
#include "concert.h"

int main(int argc, char* argv[]){
    int numOfMusicians;
    int numOfInstruments;
    int numOfConcerts;

    numOfMusicians = numOfInstruments = 0;
    InstrumentTree tr = buildTreeFromFile(argv[1], &numOfInstruments);
    Musician** MusiciansGroup = getMusiciansListFromFile(argv[2], &numOfMusicians, &tr);

    Sizes* musiCollSizes = createMusiCollSizesArr(numOfInstruments, MusiciansGroup, numOfMusicians);
    Musician*** MusiciansCollection = createMusiCollArr(musiCollSizes, numOfInstruments, MusiciansGroup, numOfMusicians);

    printTreeInOrderRec(tr.root);
    printMusiCollArrWithId(MusiciansCollection, numOfInstruments, musiCollSizes);

    Concert** concerts = getConcerts(MusiciansCollection, musiCollSizes, MusiciansGroup, numOfMusicians, &tr, &numOfConcerts);
//    printConcerts(concerts, numOfConcerts);

    freeTreeRec(tr.root);
    freePointersMusiciansArr(MusiciansGroup, numOfMusicians);
    freeMusiCollArr(MusiciansCollection, numOfInstruments);
    free(musiCollSizes);
    freeConcerts(concerts, numOfConcerts);
    return 0;
}