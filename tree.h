//
// Created by Eli on 18/04/2022.
//

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
void printTreeInOrderRec(TreeNode* root);
void freeTreeRec(TreeNode* root);

#endif //PROJECT_TREE_H
