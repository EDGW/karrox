/**
 * @file rb_tree.h
 * @brief This file provides a simple implementation of Binary-Search Tree
*/
#ifndef _LIB_BS_TREE_H
#define _LIB_BS_TREE_H
#ifndef ASM_FILE
#include "stdint.h"

/**
 * @brief a basic node 
 * @details @ref rb_tree_node extends from this
*/
struct binary_search_tree_node{
    size_t key;
    struct binary_search_tree_node* left;
    struct binary_search_tree_node* right;
    struct binary_search_tree_node* parent;
};

/**
 * @brief find the minimum child of a binary search tree node
*/
struct binary_search_tree_node* tree_minimum(struct binary_search_tree_node* x,struct binary_search_tree_node* nil);


/**
 * @brief find the maximum child of a binary search tree node
*/
struct binary_search_tree_node* tree_maximum(struct binary_search_tree_node* x,struct binary_search_tree_node* nil);
#endif //ASM_FILE
#endif //_LIB_BS_TREE_H