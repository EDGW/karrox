#include "b_search_tree.h"

/**
 * @brief find the minimum child of a binary search tree node
*/
struct binary_search_tree_node* tree_minimum(struct binary_search_tree_node* x,struct binary_search_tree_node* nil){
    while(x->left!=nil)x=x->left;
    return x;
}

/**
 * @brief find the maximum child of a binary search tree node
*/
struct binary_search_tree_node* tree_maximum(struct binary_search_tree_node* x,struct binary_search_tree_node* nil){
    while(x->right!=nil)x=x->right;
    return x;
}