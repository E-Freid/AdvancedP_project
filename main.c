#include "tree.h"
#include "musicians.h"

int main(int argc, char* argv[]){
    int numOfMusicians;
    int numOfInstruments;
    numOfMusicians = numOfInstruments = 0;
    InstrumentTree tr = buildTreeFromFile(argv[1], &numOfInstruments);
    Musician** MusiciansGroup = getMusiciansListFromFile(argv[2], &numOfMusicians, &tr);

    Sizes* musiCollSizes = createMusiCollSizesArr(numOfInstruments, MusiciansGroup, numOfMusicians);
    Musician*** MusiciansCollection = createMusiCollArr(musiCollSizes, numOfInstruments, MusiciansGroup, numOfMusicians);

    printTreeInOrderRec(tr.root);

    freeTreeRec(tr.root);
    freePointersMusiciansArr(MusiciansGroup, numOfMusicians);
    return 0;
}