#include "tree.h"
#include "musicians.h"

int main(int argc, char* argv[]){
    int numOfMusicians = 0;
    InstrumentTree tr = buildTreeFromFile(argv[1]);
    Musician** MusiciansGroup = getMusiciansListFromFile(argv[2], &numOfMusicians, &tr);

    printTreeInOrderRec(tr.root);

    freeTreeRec(tr.root);
    return 0;
}