#include "tree.h"

int main(int argc, char* argv[]){
    int size = 0;
    InstrumentTree tr = buildTreeFromFile(argv[1]);
    printTreeInOrderRec(tr.root);
    freeTreeRec(tr.root);
    return 0;
}