#include <stdio.h>
#include <stdlib.h>

typedef struct SearchTree{
    int data;
    struct SearchTree *leftChild, *rightChild;
}searchTree;


int InsertNode(searchTree** nodeAdress, int x){
    searchTree *newChild = malloc(sizeof(searchTree)), *currentChild;
    newChild->data = x;
    newChild->leftChild = NULL; 
    newChild->leftChild = NULL;
    if(*(nodeAdress)==NULL){
        *(nodeAdress) = newChild;
        return 1;
    }
    while((*(nodeAdress)!=NULL)){
        currentChild =*(nodeAdress);
        if(x<currentChild->data){
            nodeAdress=&currentChild->leftChild;
        }
        else{
            nodeAdress=&currentChild->rightChild;
        }
    }
    *nodeAdress = newChild;
    return 1;
}

searchTree* searchMax(searchTree* currentNode){
    if(currentNode!=NULL){
        while(currentNode->rightChild!=NULL){
            currentNode = currentNode->rightChild;
        }
    }
    return currentNode;
}

searchTree* searchMin(searchTree* currentNode){
    if(currentNode!=NULL){
        while(currentNode->leftChild!=NULL){
            currentNode = currentNode->leftChild;
        }
    }
    return currentNode;
}

searchTree** searchNode(searchTree* root, int x){
    searchTree *currentNode = root, **nodeAdress=&root;
    if(root!=NULL){
        while((currentNode!=NULL)&&(currentNode->data!=x)){
            if(x<currentNode->data){
                nodeAdress = &currentNode->leftChild;
                currentNode = currentNode->leftChild;
            }
            else{
                nodeAdress = &currentNode->rightChild;
                currentNode = currentNode->rightChild;
            }
        }
    }
    return nodeAdress;
}

searchTree** searchSucessor(searchTree* root, int x ){
    searchTree *currentNode = NULL, **sucessor = NULL;
    (root->data>x) ? (sucessor=&root) : (sucessor=NULL);
    currentNode=*(searchNode(root, x));
    if(currentNode!=NULL){
        if(currentNode->rightChild!=NULL){
            sucessor = &currentNode->rightChild;
            currentNode=currentNode->rightChild;
            while(currentNode->leftChild!=NULL){
                sucessor = &currentNode->leftChild;
                currentNode=currentNode->leftChild;
            }
            return sucessor;
        }
        currentNode=root;

        while(currentNode->data!=x){
            if(currentNode->data>x){
                if(currentNode->leftChild->data!=x){
                    sucessor = &(currentNode->leftChild);
                }
                currentNode=currentNode->leftChild;
            }
            else{
                currentNode=currentNode->rightChild;
            }
        }   
    }
    return sucessor;
}

searchTree** searchAntecessor(searchTree* root, int x ){
    searchTree *currentNode = NULL, **antecessor = NULL;
    (root->data<x) ? (antecessor=&root) : (antecessor=NULL);
    currentNode=*(searchNode(root, x));
    if(currentNode!=NULL){
        if(currentNode->leftChild!=NULL){
            antecessor = &currentNode->leftChild;
            currentNode=currentNode->leftChild;
            while(currentNode->rightChild!=NULL){
                antecessor = &currentNode->rightChild;
                currentNode=currentNode->rightChild;
            }
            return antecessor;
        }
        currentNode=root;

        while(currentNode->data!=x){
            if(currentNode->data<x){
                if(currentNode->rightChild->data!=x){
                    antecessor = &(currentNode->rightChild);
                }
                currentNode=currentNode->rightChild;
            }
            else{
                currentNode=currentNode->leftChild;
            }
        }   
    }
    return antecessor;
}


int removeNode(searchTree** nodeAdress, int x){
    searchTree *currentNode, *root = *(nodeAdress), *sucessor, **adressToSucessor;
    nodeAdress = searchNode(root, x);
    if(*(nodeAdress)!=NULL){
            currentNode = *(nodeAdress);
            if(currentNode->leftChild==NULL){
                *(nodeAdress) = currentNode->rightChild;
                free(currentNode);
                return 1;
            }
            if(currentNode->rightChild==NULL){
                *(nodeAdress) = currentNode->leftChild;
                free(currentNode);
                return 1;
            }
            adressToSucessor = searchSucessor(root, x);
            sucessor = *(adressToSucessor);           
            if(currentNode->rightChild==sucessor){
                *(nodeAdress)=sucessor;
                sucessor->leftChild=currentNode->leftChild;
                free(currentNode);
                return 1;
            }
            *(adressToSucessor) = sucessor->rightChild;
            currentNode->data=sucessor->data;
            free(sucessor);
            return 1;
        
    }
    return 0;
}

void freeTree(searchTree **adressToNode){
    searchTree *currentNode = *(adressToNode);
    if(currentNode->leftChild!=NULL){
        freeTree(&currentNode->leftChild);
    }
    if(currentNode->rightChild!=NULL){
        freeTree(&currentNode->rightChild);
    }
    free(currentNode);

}


int main(){

    searchTree *root = NULL, *sucessor, *max;
    InsertNode(&root, 10);
    InsertNode(&root, 9);
    InsertNode(&root, 8);
    InsertNode(&root, 7);   
    InsertNode(&root, 50);
    InsertNode(&root, 55);
    InsertNode(&root, 60);
    InsertNode(&root, 30);
    InsertNode(&root, 20);
    InsertNode(&root, 35);

    sucessor = *(searchAntecessor(root, 30));
    max  = searchMin(root);

    freeTree(&root);
    
    removeNode(&root, 30);
    removeNode(&root, 9);
    removeNode(&root, 8);
    removeNode(&root, 7);
    removeNode(&root, 50);
    removeNode(&root, 55);
    removeNode(&root, 60);
    removeNode(&root, 30);
    removeNode(&root, 20);
    removeNode(&root, 35);
    removeNode(&root, 10);

    return 1;
}