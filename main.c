#include "tree.h"

int main(int argc, char* argv[]){
    int size = 0;
    char** strArr = txtToArray(argv[1], &size);

    InstrumentTree tr = createBST(strArr, size);
    printTreeInOrderRec(tr.root);

    freeTreeRec(tr.root);
    free(strArr);
    return 0;
}