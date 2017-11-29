#include <stdio.h>
#include <stdlib.h>

#include "avl_demo.h"

struct AvlNode{
        ElementType Element;
        AvlTree Left;
        AvlTree Right;
        int Height;
};

static int Height(AvlTree P)
{
        if(P == NULL)
                return -1;
        return P->Height;

}

static int Max(int a, int b)
{
        return a>=b ? a:b;
}

/* This function can be called only if K2 has a left child 
 * Perform a rotate between a node(K2) ans lts left child
 * Update heignt, then return new root */

static Position 
SingleRotateWithLeft(Position K2)
{
        Position K1;
        K1 = K2->Left;
        K1->Right = K2;
        K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;

        K1->Height = Max(Height(K1->Left), K2->Height) + 1;

        return K1;/*New root*/
}

/*Same as SingleRotateWithLeft */
static Position 
__SingleRotateWithRight(Position K2)
{
        Position K1;
        K1 = K2->Right;
        K1->Left= K2;
        K2->Height = Max(Height(K2->Right), Height(K2->Left)) + 1;

        K1->Height = Max(Height(K1->Right), K2->Height) + 1;

        return K1;/*New root*/
}

/*Same as SingleRotateWithLeft */
static Position 
SingleRotateWithRight(Position K1)
{
        Position K2;
        K2 = K1->Right;
        K1->Right = K2->Left;
        K2->Left= K1;

        K2->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;

        K1->Height = Max(Height(K2->Right), K1->Height) + 1;

        return K2;/*New root*/
}

/*This function can be called only if K3 has a left
 * child and K3's left child has right child 
 * Do the left-right double rotate 
 * Update height, then return new root. */

static Position DoubleRotateWithLeft(Position K3)
{
        /*Rotate between K1 and K2*/
        K3->Left = SingleRotateWithRight(K3->Left);

        /*Rotate between K3 and K2*/
        return SingleRotateWithLeft(K3);
}

/*Same as Position DoubleRotateWithLeft*/
static Position DoubleRotateWithRight(Position K3)
{
        /*Rotate between K1 and K2*/
        K3->Right = SingleRotateWithLeft(K3->Right);

        /*Rotate between K3 and K2*/
        return SingleRotateWithRight(K3);
}

AvlTree Insert(ElementType X, AvlTree T)
{
        printf("-------- X = %d --------\n", X);
        if(T == NULL){
                printf("...1...\n");
                /*Create and return one-node tree */
                T = (AvlTree)malloc(sizeof(struct AvlNode));
                if(!T){
                        printf("err out of space.\n");
                        //return NULL;
                }else{
                        T->Element = X;
                        T->Height = 0;
                        T->Left = T->Right = NULL;
                }

        }else if(X < T->Element){
                printf("...2.0... T->Left=%x T->Element=%d \n", T->Left,T->Element);
                T->Left = Insert(X, T->Left);
                printf("...2.1... T->Left=%x \n", T->Left);
                if(Height(T->Left) - Height(T->Right) == 2){
                        printf("...2.2... T->Left=%x \n", T->Left);
                        if(X < T->Left->Element)
                                T = SingleRotateWithLeft(T);
                        else
                                T = DoubleRotateWithLeft(T);
                }
                printf("...2.3...\n");
        }else if(X > T->Element){
                printf("...3.0... T->Right=%x T->Element=%d \n", T->Right,T->Element);
                T->Right = Insert(X, T->Right);
                printf("...3.1... T->Right=%x \n", T->Right);
                if(Height(T->Right) - Height(T->Left) == 2){
                        printf("...3.2... T->Right=%x \n", T->Right);
                        if(X < T->Right->Element){
                                printf("...3.3... T->Right->Element=%d \n", T->Right->Element);
                                T = SingleRotateWithRight(T);
                        }else{
                                printf("...3.4... T->Right->Element=%d \n", T->Right->Element);
                                T = DoubleRotateWithRight(T);
                        }
                }
                printf("...3.5...\n");
        }
        printf("-------- 4 --------\n");
        /*Else X is in the tree already; we'll do nothing */
        T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
        return T;
}

/*Traverse.*/
void PreOrderTraverse(AvlTree T)
{
        if(T != NULL){
                printf("%3d:%2d(%3d, %3d)  ", T->Element, T->Height,
                       T->Left==NULL?-1:T->Left->Element,
                       T->Right==NULL?-1:T->Right->Element);
                PreOrderTraverse(T->Left);
                PreOrderTraverse(T->Right);
        }
}

int main(int argc, const char **argv)
{
        int i;
        AvlTree avl_demo;
        avl_demo = (AvlTree)malloc(sizeof(struct AvlNode));
        if(!avl_demo)
                return -1;
        //for(i=0; i<15; i++){
                Insert(100, avl_demo);
                Insert(30, avl_demo);
                Insert(3, avl_demo);
                Insert(4, avl_demo);
                Insert(5, avl_demo);
                Insert(6, avl_demo);
                Insert(7, avl_demo);
        //} 
        PreOrderTraverse(avl_demo);
        printf("\n");

        return 0;
}
