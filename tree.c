#include "tree.h"

int findInstId(InstrumentTree* tree, char *instrument){
    return findInstIdRec(tree->root, instrument);
}
int findInstIdRec(TreeNode* root, char *instrument){
    if(!root)
        return NOT_FOUND;

    if(strcmp(instrument,root->instrument) == 0)
        return root->insId;

    else if(strcmp(instrument,root->instrument) > 0)
        return findInstIdRec(root->right, instrument);

    else
        return findInstIdRec(root->left, instrument);
}

void makeEmptyTree(InstrumentTree* tr){
    tr->root = NULL;
}

TreeNode* createTreeNode(char* string, int id, TreeNode* left, TreeNode* right){
    TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));
    checkAllocation(node);

    node->instrument = string;
    node->insId = id;
    node->left = left;
    node->right = right;

    return node;
}


void printTreeInOrderRec(TreeNode* root){
    if(!root)
        return;

    else {
        printTreeInOrderRec(root->left);
        printf("%d ", root->insId);
        puts(root->instrument);
        printTreeInOrderRec(root->right);
    }
}

void freeTreeRec(TreeNode* root){
    if(!root)
        return;

    else {
        freeTreeRec(root->left);
        freeTreeRec(root->right);
        free(root->instrument);
        free(root);
    }
}

InstrumentTree buildTreeFromFile(char* filePath){
    InstrumentTree tr;
    int id = 0;
    char str[MAX_LINE_LENGTH];
    char* formattedStr;
    BOOL reachedEOF = FALSE;

    makeEmptyTree(&tr);
    FILE* f = fopen(filePath, "r");
    checkOpen(f);

    while(!reachedEOF){
        fgets(str, MAX_LINE_LENGTH, f);
        if(feof(f))
            reachedEOF = TRUE;   // finished reading the file, exit the loop
        else {
            formattedStr = formatRawStr(str);
            insertStrToBinaryTree(&tr, formattedStr, id);
            id++;
        }
    }

    fclose(f);
    return tr;
}

void insertStrToBinaryTree(InstrumentTree* tr, char* str, int id){
    TreeNode* newNode = createTreeNode(str, id, NULL, NULL);
    InsertNodeToBST(tr, newNode);
}

void InsertNodeToBST(InstrumentTree* tr, TreeNode* newNode) {
    int cmpr;
    TreeNode *root = tr->root;
    BOOL placedNode = FALSE;

    if (!root) {        // empty tree
        tr->root = newNode;
        return;
    }

    while (root && !placedNode) {
        cmpr = strcmp(root->instrument, newNode->instrument);
        if (cmpr < 0) {         // newNode is smaller
            if (!root->right) {
                root->right = newNode;          //  ptr is NULL, found location
                placedNode = TRUE;
            } else
                root = root->right;     // ptr not NULL, go right
        } else {
            if (!root->left) {
                root->left = newNode;
                placedNode = TRUE;
            } else
                root = root->left;
        }
    }
}