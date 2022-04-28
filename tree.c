//
// Created by Eli on 18/04/2022.
//

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

void mergeSort(char** instruArray, int size){
    if(size <= 1)
        return;
    mergeSort(instruArray, size/2);
    mergeSort(instruArray + size/2, (size+1)/2);
    char** res = (char**) malloc(sizeof(char*)*size);
    checkAllocation(res);
    merge(instruArray,size/2,instruArray+size/2,(size+1)/2, res);
    copy(instruArray, res, size);
    free(res);
}

void merge(char** arr1, int size1, char** arr2, int size2, char** res){
    int ind1,ind2,indRes,cmpr;
    ind1 = ind2 = indRes = 0;
    while(ind1 < size1 && ind2 < size2){
        cmpr = strcmp(arr1[ind1],arr2[ind2]);
        if(cmpr < 0){
            res[indRes] = arr1[ind1];
            ind1++, indRes++;
        }
        else{
            res[indRes] = arr2[ind2];
            ind2++, indRes++;
        }
    }
    while(ind1 < size1){
        res[indRes] = arr1[ind1];
        ind1++, indRes++;
    }
    while(ind2 < size2){
        res[indRes] = arr2[ind2];
        ind2++, indRes++;
    }
}

void copy(char** arr, char** source, int size){
    for(int i=0;i<size;i++){
        arr[i] = source[i];
    }
}

InstrumentTree createBST(char** instruArray, int size){
    int id = 0;
    InstrumentTree tr;
    makeEmptyTree(&tr);
    mergeSort(instruArray, size);
    tr.root = createBSTRec(instruArray, size, &id);
    return tr;
}

TreeNode* createBSTRec(char** instruArray, int size, int* id){
    if(size == 0)
        return NULL;

    TreeNode *left,*right;
    int midIndex;

    if(size % 2 == 0){
        left = createBSTRec(instruArray, size/2 - 1, id);
        right = createBSTRec(instruArray + size/2 ,size/2, id);
        midIndex = size/2 - 1;
    }
    else{
        left = createBSTRec(instruArray, size/2, id);
        right = createBSTRec(instruArray + (size+2)/2, size/2, id);
        midIndex = size/2;
    }
    TreeNode* root = createTreeNode(instruArray[midIndex], *id, left, right);
    *id += 1;
    return root;
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

// test