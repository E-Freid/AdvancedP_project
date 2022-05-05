#ifndef PROJECT_TREE_H
#define PROJECT_TREE_H

#include "header.h"

typedef struct treeNode{
    char* instrument;
    unsigned short insId;
    struct treeNode* left;
    struct treeNode* right;
} TreeNode;

typedef struct tree{
    TreeNode* root;
} InstrumentTree;

int findInsId(InstrumentTree tree, char *instrument);
int findInstIdRec(TreeNode* root, char *instrument);
void makeEmptyTree(InstrumentTree* tr);
TreeNode* createTreeNode(char* string, int id, TreeNode* left, TreeNode* right);
void mergeSort(char** instruArray, int size);
void merge(char** arr1, int size1, char** arr2, int size2, char** res);
void copy(char** arr, char** source, int size);
InstrumentTree createBST(char** instruArray, int size);
TreeNode* createBSTRec(char** instruArray, int size, int* id);
void printTreeInOrderRec(TreeNode* root);
void freeTreeRec(TreeNode* root);

#endif //PROJECT_TREE_H
