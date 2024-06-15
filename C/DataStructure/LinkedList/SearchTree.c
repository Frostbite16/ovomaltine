#include <stdio.h>
#include <stdlib.h>

typedef struct sTree{
    int data:32;
    int *proxL;
    int *proxR;
}sTreeNode;

sTreeNode* insertSTree(sTreeNode* root, int x){
    sTreeNode* c = malloc(sizeof(sTreeNode)), *p=root;

    if(root==NULL){
        c->data=x;
        c->proxL=NULL;
        c->proxR=NULL;
        return c;
    }
    while(p->data>x){
       
    }

}


