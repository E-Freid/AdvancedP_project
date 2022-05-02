#include "tree.h"


int findInstId(InstrumentTree tree, char *instrument){
    return findInstIdRec(tree.root, instrument);
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
    checkAllocation(node->instrument);
    node->insId = id;
    node->left = left;
    node->right = right;

    return node;
}


void printTreeInOrderRec(TreeNode* root){
    if(!root)
        return;
    else{
        printTreeInOrderRec(root->left);
        printf("%d ", root->insId);
        puts(root->instrument);
        printTreeInOrderRec(root->right);
    }
}

void freeTreeRec(TreeNode* root){
    if(!root)
        return;
    else{
        freeTreeRec(root->left);
        freeTreeRec(root->right);
        free(root->instrument);
        free(root);
    }
}