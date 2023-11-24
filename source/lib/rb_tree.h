/**
 * @file rb_tree.h
 * @brief This file provides a simple implementation of Red-Black Tree
*/
#ifndef _LIB_RB_TREE_H
#define _LIB_RB_TREE_H
#ifndef ASM_FILE
#include "stdint.h"

typedef uint8_t rb_tree_color;
#define RB_TREE_COLOR_BLACK 0
#define RB_TREE_COLOR_RED   1

/**
 * @brief a rb-tree node, extends from a binary search tree node
*/
struct rb_tree_node{
    size_t key;
    struct rb_tree_node* left;
    struct rb_tree_node* right;
    struct rb_tree_node* parent;
    /*  the defined properties are compatible with binary search tree
        to maintain the compability with binary search tree functions
        it's not recommended to use binary search tree functions on rb-tree,
        it may break the properties of the rb-tree
    */
    rb_tree_color color;
};


struct rb_tree{
    struct rb_tree_node* root_node;
};


/**
 * @brief initialize a tree
*/
void init_rb_tree(struct rb_tree* tree);

/**
 * @brief to check if whether a node is null
*/
int rb_is_null(struct rb_tree_node* x);

/**
 * @brief insert a node to the rb-tree
*/
void rb_insert(struct rb_tree* tree,struct rb_tree_node* z);

/**
 * @brief detele a node from the rb-tree
*/
void rb_delete(struct rb_tree* tree,struct rb_tree_node* z);
#endif //ASM_FILE
#endif //_LIB_RB_TREE_H