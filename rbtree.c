/* 
   
   Properties:
   1. Every node is R or B
   2. Root is B.
   3. Every leaf (sentinel node) is B.
   4. If a node is R, then both its children are B.
   5. All simple paths from node to leaf contain the same number of B nodes.
   
***IMPORTANT***
   Implementation notes:
   1. Root's parent is sentinel node, if root is a node. 
   2. Leaf nodes are also sentinel node. 
   3. All RB Tree instances will share this sentinel node for their root's parent and leaves.
   4. Empty root is initialized to sentinel.
   5. Unsuccessful search (rb_search)  always returns the sentinel node. 
   
   Implementation based on CLRS 3rd edition.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rbtree.h"
#define BLACK 0
#define RED 1
#define SENTINEL_KEY "NIL"


struct rb_node *SENTINEL(){

	static struct rb_node *sentinel;
	if (sentinel == NULL){
		sentinel = (struct rb_node *) malloc(sizeof(struct rb_node));
		sentinel->color =  BLACK;
		sentinel->left = NULL;
		sentinel->right = NULL;
		sentinel->parent = NULL;
		sentinel->key = SENTINEL_KEY;
	}
	return sentinel;
}


extern struct rb_tree *rb_tree_alloc(){

	struct rb_tree* tree;
	tree = (struct rb_tree*) malloc(sizeof(struct rb_tree));
	memset(tree, 0, sizeof(tree));
	tree->root = SENTINEL();
	return tree;
}


/*
      |                   |
      x                   y
     / \     ====>       / \
    a   y               x   c 
       / \             / \
      b   c           a   b
      
      node: x
*/
void left_rotate(struct rb_tree *tree, struct rb_node *x){

	struct rb_node *y = x->right;
	x->right = y->left;

	if ( y->left != SENTINEL() ){
		y->left->parent = x;
	}
	y->parent = x->parent;

	if ( x->parent == SENTINEL() ){
		tree->root = y;
	
	}
	else if(x == x->parent->left){

		x->parent->left = y;
	}
	else{
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}


/*
        |                   |
        y                   x
       / \   ======>       / \
      x   c               a   y
     / \                     / \
    a   b                   b   c

*/
void right_rotate( struct rb_tree *tree, struct rb_node *y){

	struct rb_node *x =  y->left;
	y->left = x->right;

	if ( x->right != SENTINEL() ){
		x->right->parent = y;
	}
	x->parent = y->parent;

	if ( y->parent == SENTINEL() ){  /* Y is ROOT */
		tree->root = x;
	}
	else if (y == y->parent->left){ /* Y is a LEFT child */
		y->parent->left = x;
	}
	else{
		y->parent->right = x; /* Y is RIGHT child */
	}
	x->right = y;
	y->parent = x;
	
}


extern void rb_insert(struct rb_tree *tree, struct rb_node *node){

	struct rb_node *y = SENTINEL();
	struct rb_node *x = tree->root;
	
	/*traverse down the tree to find the insertion point*/
	while (x != SENTINEL()){
		y = x;
		x = (LESS_THAN(node->key, x->key)) ? x->left : x->right;
	}
	node->parent = y;

	/*Determine where to place the node relative to parent*/
	if (y == SENTINEL()){
		tree->root = node;
	}
	else if (LESS_THAN(node->key, y->key)){
		y->left = node;  
	}
	else {
		y->right = node;
	}

	node->left = SENTINEL();
	node->right = SENTINEL();
	node->color = RED;
	rb_insert_fixup(tree, node);
}


void rb_insert_fixup(struct rb_tree* tree, struct rb_node* node){

	struct rb_node* y;

	while (node->parent != SENTINEL() && node->parent->color == RED){
		if (node->parent == node->parent->parent->left){
			y = node->parent->parent->right;
			/*case 1: node's uncle y is red*/
			if (y->color == RED) {  
				node->parent->color = BLACK;
				y->color = BLACK;
				node->parent->parent->color = RED;
				 /* Check if nodes grand parent needs fixup*/
				node = node->parent->parent;
			}
			/*case 2: node's uncle y is black and node is a right child*/
			else if (node == node->parent->right){
				/*left rotate parent*/
				node = node->parent;
				left_rotate(tree, node);
			}

			/*case 3: node's uncle y is black and node is a left child*/
			node->parent->color = BLACK;
			node->parent->parent->color = RED;
			right_rotate(tree, node->parent->parent);
					
		}
		else {
			/*symmetric case of above except that node's parent is the right child of GP.*/
			y = node->parent->parent->left;
			/*case 1*/
			if (y->color == RED){
				node->parent->color = BLACK;
				y->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}/*case 2*/
			else {
			  if (node == node->parent->left){
				node = node->parent;
				right_rotate(tree, node);
			  }
			/*case 3*/
			  node->parent->color = BLACK;
			  node->parent->parent->color = RED;
			  left_rotate(tree, node->parent->parent);
			}
		}
		
	}

	tree->root->color = BLACK;
}



void rb_transplant(struct rb_tree* tree, struct rb_node* u, struct rb_node* v){

	if (u->parent == SENTINEL()){
		tree->root = v;
	}
	else if(u == u->parent->left){
		u->parent->left = v;
	}
	else {
		u->parent->right = v;
	}
	v->parent = u->parent;
}


extern struct rb_node* rb_delete(struct rb_tree* tree, struct rb_node* node){

	struct rb_node* x;
	struct rb_node* y = node;
	unsigned int y_original_color = y->color;

	if (node->left == SENTINEL()){
		x = node->right;
		rb_transplant(tree, node, node->right);
	}
	else if(node->right == SENTINEL()){
		x = node->left;
		rb_transplant(tree, node, node->left);
	}
	else { /* node has two children that are not sentinel*/
		
		y = tree_minimum(node->right);
		y_original_color = y->color;
		x = y->right;

		/*simple case where the tree minimum is node's right child*/
		if (y->parent == node){
			x->parent = y;
		}
		else {  /* make tree minimum the replacement for node 
			   and its right subtree is nodes right subtree
			   with tree minimum spliced out and tree minimums right subtree
			   is replacement node's rights left subtree. 
			*/
			rb_transplant(tree, y, y->right);
			y->right = node->right;
			y->right->parent = y;
		}
		/*tree minimum is node's right child*/
		rb_transplant(tree, node, y);
		y->left = node->left;
		y->left->parent = y;
		y->color = node->color;
	}
	if (y_original_color == BLACK){
		rb_delete_fixup(tree, x);
	}

	return x;
}


void rb_delete_fixup(struct rb_tree *tree, struct rb_node *node){

	struct rb_node *x, *w;

	
	while (node != tree->root && node->color == BLACK){
		if (node == node->parent->left){
			w = node->parent->right;
			/*case 1: node's sibling w is red. Switch colors of parent
			 and sibling and perform left rotation on the parent.*/
			if (w->color == RED){
				w->color = BLACK;
				node->parent->color = RED;
				left_rotate(tree, node->parent);
				w = node->parent->right;
			}
			/*case 2: node's sibling is black and both of siblings children are black
			  Mark the sibling red and the new node is now the parent.
			 */
			
			if (w->left->color == BLACK && w->right->color == BLACK){
				w->color = RED;
				node = node->parent;
			}
			/*case 3: node's sibling is black. Siblings left child is red, 
			  and right child is black. Switch colors of sibling and its left child
			  and perform right rotate on sibling.*/
			else if (w->right->color == BLACK){
				w->left->color = BLACK;
				w->color = RED;
				right_rotate(tree, w);
				w = node->parent->right;
			}
			/*case 4: sibling is black and its right child is red.
			  Make siblings right black and nodes paren't black.
			  Sibling gets the same color as parent. Perform left rotate on parent.
			 */
			w->color = node->parent->color;
			node->parent->color = BLACK;
			w->right->color = BLACK;
			left_rotate(tree, node->parent);
			node = tree->root;
		}
		else {
			/*Symmetric case where node is parent's right child.*/
			w = node->parent->left;
			if (w->color == RED){
				w->color = BLACK;
				node->parent->color = RED;
				right_rotate(tree, node->parent);
				w = node->parent->left;
			}
			if (w->left->color == BLACK &&\
			    w->right->color == BLACK){
				w->color = RED;
				node = node->parent;
			}
			else if (w->left->color == BLACK){
				w->right->color = BLACK;
				w->color = RED;
				left_rotate(tree, w);
				w = node->parent->left;
			}

			w->color = node->parent->color;
			node->parent->color = BLACK;
			w->right->color = BLACK;
			right_rotate(tree, x->parent);
			node = tree->root;
		}
	}
	node->color = BLACK;
}


extern struct rb_node* rb_search(struct rb_tree* tree, char* key){

	struct rb_node* node = tree->root;
	
	while (node != SENTINEL() &&\
	       NOT_EQUAL(node->key, key)){

		node = LESS_THAN(key, node->key) ? node->left : node->right;
	}

	return node;
}


extern struct rb_node* tree_minimum(struct rb_node* node){

	while (node->left != SENTINEL()){
		node = node->left;
	}
	return node;
}


extern struct rb_node* tree_maximum(struct rb_node* node){

	while (node->right != SENTINEL()){
		node = node->right;
	}
	return node;
}


extern struct rb_node* tree_successor(struct rb_node* node){

	struct rb_node* y;

	if (node->right != SENTINEL())
		return tree_minimum(node->right);

	y = node->parent;

	while (y != SENTINEL() && node == y->right){

		node = y;
		y = y->parent;
	}

	return y;
}

extern struct rb_node* tree_predecessor(struct rb_node* node){

	struct rb_node* y;

	if (node->left != SENTINEL())
		return tree_maximum(node->left);

	y = node->parent;

	while (y != SENTINEL() && node == y->left){
		node = y;
		y = y->parent;
	}

	return y;
}

					
extern struct rb_node* rb_node_alloc(struct rb_node* parent, struct rb_node* left, \
				     struct rb_node* right, char* key, char* data){

	struct rb_node* node = malloc(sizeof(struct rb_node));
	node->key = (char *) malloc((strlen(key) + 1) * sizeof(char));
	node->parent = SENTINEL();
	node->left = SENTINEL();
	node->right = SENTINEL();
	node->data = data;
	strcpy(node->key, key);
	strcat(node->key, "\0");
	return node;
}
				     
extern struct rb_node* rb_node_alloc_kv(char* key, char* value){

	struct rb_node* node = (struct rb_node *)  malloc(sizeof(struct rb_node));
	node->key = (char *) malloc((strlen(key) + 1) * sizeof(char));
	node->data = (char *) malloc((strlen(key) + 1) * sizeof(char));
	strcpy(node->key, key);
	strcat(node->key, "\0");
	strcpy(node->data, value);

	return node;
}


extern bool LESS_THAN(char *a, char *b){

	size_t i, j;
	
	if (strlen(a) < strlen(b)) return true;
	else if(strlen(a) > strlen(b)) return false;

	for (i = 0, j = 0; i < strlen(a); i++, j++){
		if (a[i] < b[j]) return true;
		else if (a[i] > b[j]) return false;
	}
	return false;
}


extern bool NOT_EQUAL(char *a, char *b){

	size_t i, j;

	if (strlen(a) != strlen(b))
		return true;

	for (i = 0, j = 0; i < strlen(a); i++, j++){
		if (a[i] != b[j])
			return true;
	}
	return false;
}


extern void __LIST_KEYS_SORTED(struct rb_node* node){

	if (node == SENTINEL()) return;
	__LIST_KEYS_SORTED(node->left);
	__PRINT_NODE(node);
	__LIST_KEYS_SORTED(node->right);
}


extern void __PRINT_NODE(struct rb_node* node){
  
	printf(
	       "Node: %s, Color: %d, P: %s, LC: %s, RC: %s\n",	\
	       node->key,
	       node->color,
	       node->parent->key,
	       node->left->key,
	       node->right->key
	       );
}



extern void set(struct rb_tree *tree, char *key, char* data){


	struct rb_node* candidate = rb_search(tree, key);

	if (candidate != SENTINEL()){
		candidate->data = data;
	}
	else{
		struct rb_node* new_node = rb_node_alloc_kv(key, data);
		rb_insert(tree, new_node);
	}
	
}


extern bool delete(struct rb_tree *tree, char *key){

	struct rb_node *candidate = rb_search(tree, key);

	if (candidate != NULL && candidate != SENTINEL()){
		rb_delete(tree, candidate);
		rb_free(candidate);
		return true;
	}

	return false;
}


extern bool is_member(struct rb_tree* tree, char* key){

	struct rb_node *candidate = rb_search(tree, key);

	if (candidate != SENTINEL())
		return true;
	return false;
	
}


extern void rb_free(struct rb_node* node){

	free(node->data);
	free(node->key);
	free(node);
}


void _print_tree_recursive(struct rb_node* node){
        if (!node || node == SENTINEL())
		return;
	__PRINT_NODE(node);
	_print_tree_recursive(node->left);
	_print_tree_recursive(node->right);
}

void _print_tree(struct rb_tree* tree){
	_print_tree_recursive(tree->root);

}
