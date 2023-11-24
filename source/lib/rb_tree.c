#include "rb_tree.h"
#include "b_search_tree.h"

/**
 * @brief defines a nil node instead of NULL
*/
static struct rb_tree_node nil_node_struct = {// extern "rb_tree.h"
    0,//key
    0,//left
    0,//right
    0,//parent
    RB_TREE_COLOR_BLACK,//color
};
static struct rb_tree_node *nil_node = &nil_node_struct;

/**
 * @brief initialize a tree
*/
void init(struct rb_tree* tree){
    tree->root_node=nil_node;
}

/**
 * @brief to check if whether a node is null
*/
int rb_is_null(struct rb_tree_node* x){
    return x == nil_node;
}

/**
 * @brief left-rotate a tree node,
 * @brief it must be guaranteed that the node x->right exists.
*/
void rb_left_rotate(struct rb_tree* tree,struct rb_tree_node* x){//extern "rb_tree.c"
    /*
         X                  Y
        / \                / \
       A   Y     --->     X   C
          / \            / \
        [B]  C          A  [B]
    
    */
    struct rb_tree_node* y = x->right;
    x->right=y->left;
    if(y->left!=nil_node){
        y->left->parent=x;
    }
    y->parent=x->parent;
    if(x->parent==nil_node){//indicates x is the root of the tree
        tree->root_node=y;
    }else if(x==x->parent->left){//x is the left child of it's parent
        x->parent->left=y;
    }else{//x is the right child of it's parent
        x->parent->right=y;
    }
    y->left=x;
    x->parent=y;
}

/**
 * @brief right-rotate a tree node,
 * @brief it must be guaranteed that the node x->left exists.
*/
void rb_right_rotate(struct rb_tree* tree,struct rb_tree_node* x){
    /*
             X             Y
            / \           / \
           Y   C  --->   A   X
          / \               / \
         A  [B]           [B]  C
    */
    struct rb_tree_node *y = x->left;
    x->left = y->right;
    if(y->right!=nil_node){
    y->right->parent=x;
    }
    y->parent=x->parent;
    if(x->parent==nil_node){//indicates x is the root of the tree
        tree->root_node=y;
    }else if(x==x->parent->left)x->parent->left=y;
    else x->parent->right=y;
    y->right=x;
    x->parent=y;
}

/**
 * @brief fixup after insertion to maintain rb-tree properties
*/
void rb_insert_fixup(struct rb_tree* tree,struct rb_tree_node* z){
/*
    the loop invariant:
    1.z is a red node
    2.if z.p is the root of the tree, it's black
    3.the broken properties of this rb-tree shall be only one of the followings:
        (a) the root is black
        (b) the children of a node are black when the node is red. 
*/
    /*
    property b are broken
    this also indicates the existence of z.father & z.grandfather
    */
    while(z->parent->color==RB_TREE_COLOR_RED){
        if(z->parent==z->parent->parent->left){
            struct rb_tree_node* y=z->parent->parent->right; //z's uncle
            if(y->color==RB_TREE_COLOR_RED){    //case 1 y.color is red
                /*
                        B                R   
                       / \              / \  
                      R   R(y)  --->   B   B(y)
                      |                |      
                     R(z)             R(z)   
                    then fix z.grandfather
                */
                z->parent->color=RB_TREE_COLOR_BLACK;
                y->color=RB_TREE_COLOR_BLACK;
                z->parent->parent->color=RB_TREE_COLOR_RED;
                z=z->parent->parent;
            }else{
                if(z==z->parent->right){  //case 2: y.color is black and z is a right-son
                    /*
                            B                         B       
                           / \                       / \      
                          C  B(y)     --->          R  B(y)  
                         / \     rotate to case 3  / \        
                        A  R(z)   and change z   C(z) E
                           / \                   / \
                          D   E                 A   D   
                    */
                   z=z->parent;
                   rb_left_rotate(tree,z);
                }
                //case 3:y.color is black and z is a left-son
                /*
                           B(F)                  B(C)
                           / \                  /    \
                         R(C) B(y)            R(z)   B(F)
                         / \        --->      / \    / \  
                       R(z) E                A   D  E  B(y)
                      / \   
                     A   D
                */
               z->parent->color=RB_TREE_COLOR_BLACK;
               z->parent->parent->color=RB_TREE_COLOR_RED;
               rb_right_rotate(tree,z->parent->parent);
            }
        }else{
            struct rb_tree_node* y=z->parent->parent->left;
            if(y->color==RB_TREE_COLOR_RED){
                z->parent->color=RB_TREE_COLOR_BLACK;
                y->color=RB_TREE_COLOR_BLACK;
                z->parent->parent->color=RB_TREE_COLOR_RED;
                z=z->parent->parent;
            }else{
                if(z==z->parent->left){
                   z=z->parent;
                   rb_right_rotate(tree,z);
                }
               z->parent->color=RB_TREE_COLOR_BLACK;
               z->parent->parent->color=RB_TREE_COLOR_RED;
               rb_left_rotate(tree,z->parent->parent);
            }
        }
    }
    tree->root_node->color = RB_TREE_COLOR_BLACK;//property a are broken
}

/**
 * @brief insert a node to the rb-tree
*/
void rb_insert(struct rb_tree* tree,struct rb_tree_node* z){
    
    struct rb_tree_node* y = nil_node;
    struct rb_tree_node* x = tree->root_node;
    while(x!=nil_node){
        y=x;
        if(z->key < x->key)x=x->left;
        else x=x->right;
    }
    //y: the current leaf node
    z->parent=y;
    if(y==nil_node){//it's an empty tree
        tree->root_node=z;
    }
    else if(z->key<y->key)y->left=z;
    else y->right=z;
    z->left=nil_node;
    z->right=nil_node;
    z->color=RB_TREE_COLOR_RED; // default insert color is red.
    rb_insert_fixup(tree,z);
}

/**
 * @brief use a subtree rooted v to replace u, and update u's parent's child value
*/
void rb_transplant(struct rb_tree* tree,struct rb_tree_node* u,struct rb_tree_node* v){
    /*
        the properties of v is never used
        it doesn't matter if v is nil.
    */
    if(u->parent==nil_node){//u is the root
        tree->root_node=v;
    }else if(u==u->parent->left){
        u->parent->left=v;
    }else u->parent->right=v;
    v->parent=u->parent;
}

/**
 * @brief fixup after deletion to maintain rb-tree properties
*/
void rb_delete_fixup(struct rb_tree* tree,struct rb_tree_node* x){
    /*
    the following properties might be broken after a deletion.
    1.The root would be RED when node y is the root and the successor of y is RED.
    2.Both node x and its parent are RED.
    3.The number of black nodes on the path from the root node to any one leaf node is not equal.
    */
    while(x!=tree->root_node && x->color==RB_TREE_COLOR_BLACK){
        struct rb_tree_node* w;//the sibling of x
        if(x==x->parent->left){
            w=x->parent->right;
            /* Case 1
            []: Red Node
                        B                    D
                       / \                  / \
                    x A  [D] w  ----->    [B]  E
                         / \              / \
                        C   E          x A   C new w
                    transform case 1 to case 2,3,4
            */
            if(w->color==RB_TREE_COLOR_RED){
                w->color=RB_TREE_COLOR_BLACK;
                x->parent->color=RB_TREE_COLOR_RED;
                rb_left_rotate(tree,x->parent);
                w = x->parent->right;
            }
            /* Case 2
                        [B]                     B new x
                        / \                    / \
                     x A   D w     ---->      A   D 
                          / \ jump to its parent / \
                         C   E                  C   E
            */
            if(w->left->color==RB_TREE_COLOR_BLACK 
                && w->right->color==RB_TREE_COLOR_BLACK){
                    w->color=RB_TREE_COLOR_RED;
                    x=x->parent;
            }
            else{
                /* Case 3
                       B                        B
                      / \                      / \
                   x A   D w       ---->    x A   C new w
                        / \                        \
                      [C]  E                       [D]
                                                     \
                                                      E
                        transform from case 3 to case 4
                */
                if(w->right->color==RB_TREE_COLOR_BLACK){
                    w->left->color=RB_TREE_COLOR_BLACK;
                    w->color=RB_TREE_COLOR_RED;
                    rb_right_rotate(tree,w);
                    w=x->parent->right;
                }
                /* Case 4
                       B                      D
                      / \                    / \
                   x A   D w      ---->     B   E
                        / \                / \
                       C  [E]             A   C
                */
                w->color=x->parent->color;
                x->parent->color=RB_TREE_COLOR_BLACK;
                w->right->color=RB_TREE_COLOR_BLACK;
                rb_left_rotate(tree,x->parent);
                x=tree->root_node;
            }
        }else{
            w=x->parent->left;
            if(w->color==RB_TREE_COLOR_RED){
                w->color=RB_TREE_COLOR_BLACK;
                x->parent->color=RB_TREE_COLOR_RED;
                rb_right_rotate(tree,x->parent);
                w = x->parent->left;
            }
            if(w->right->color==RB_TREE_COLOR_BLACK 
                && w->left->color==RB_TREE_COLOR_BLACK){
                    w->color=RB_TREE_COLOR_RED;
                    x=x->parent;
            }else{
                if(w->left->color==RB_TREE_COLOR_BLACK){
                    w->right->color=RB_TREE_COLOR_BLACK;
                    w->color=RB_TREE_COLOR_RED;
                    rb_left_rotate(tree,w);
                    w=x->parent->left;
                }
                w->color=x->parent->color;
                x->parent->color=RB_TREE_COLOR_BLACK;
                w->left->color=RB_TREE_COLOR_BLACK;
                rb_right_rotate(tree,x->parent);
                x=tree->root_node;
            }
        }
    }
    x->color=RB_TREE_COLOR_BLACK;
}

/**
 * @brief detele a node from the rb-tree
*/
void rb_delete(struct rb_tree* tree,struct rb_tree_node* z){
    struct rb_tree_node* y=z;
    struct rb_tree_node* x;
    rb_tree_color y_original_clr = y->color;
    /*
        X           X    
         \     or    \     ---->    R or NIL
          R          NIL  
    */
    if(z->left==nil_node){
        x=z->right;
        rb_transplant(tree,z,z->right);
    }
    /*
          X                 
         /        ->>       L
        L
    */
    else if(z->right==nil_node){
        x=z->left;
        rb_transplant(tree,z,z->left);
    }
    /*        P                                     P
             /                                     /
            Z                                     Y
           / \                                   / \
         ...  A                                ...  A
             ...                ----->             ...      
            /         trans y.right ->y           /
           B          trans y   ->    z          B
          /                                       \
         Y  -> the successor                      ...
          \    of z                       
          ...        
    */
    else{
        y = (struct rb_tree_node*)  // get z's successor
            tree_minimum((struct binary_search_tree_node*)(z->right)
                        ,(struct binary_search_tree_node*) nil_node);
        y_original_clr = y->color;
        x=y->right;
        if(y!=z->right){
            rb_transplant(tree,y,y->right);    //trans y.right -> y
            y->right=z->right;
            y->right->parent=y;
        }
        else x->parent=y;   //in case x is nil.
        rb_transplant(tree,z,y);            // trans y->z
        y->left=z->left;
        y->left->parent=y;
        y->color=z->color;
    }
    if(y_original_clr==RB_TREE_COLOR_BLACK) // black count decreased
        rb_delete_fixup(tree,x);            // correct it.
}
