/**/
#include <stdbool.h>

struct rb_node{

	struct rb_node* parent;
	struct rb_node* left;
	struct rb_node* right;
	char* key;
	char* data;
	unsigned int color:1;
};

struct rb_tree{
	struct rb_node* root;
};

struct rb_node* SENTINEL();

extern struct rb_node* tree_minimum(struct rb_node*);

extern struct rb_node* tree_maximum(struct rb_node*);

extern struct rb_node* tree_successor(struct rb_node*);

extern struct rb_node* tree_predecessor(struct rb_node*);

extern void __LIST_KEYS_SORTED(struct rb_node*);

extern void __PRINT_NODE(struct rb_node*);

void left_rotate(struct rb_tree*, struct rb_node*);

void right_rotate(struct rb_tree*, struct rb_node*);

extern void rb_insert(struct rb_tree*, struct rb_node*);

void rb_insert_fixup(struct rb_tree*, struct rb_node*);

extern struct rb_node* rb_delete(struct rb_tree*, struct rb_node*);

extern struct rb_node* rb_search(struct rb_tree*, char*);

void rb_delete_fixup(struct rb_tree*, struct rb_node*);

void rb_transplant(struct rb_tree*, struct rb_node*, struct rb_node*);

struct rb_tree* rb_tree_alloc();

struct rb_node* rb_node_alloc(struct rb_node*, struct rb_node*, struct rb_node*, char*, char*);

struct rb_node* rb_node_alloc_kv(char*, char*);

struct rb_node* search(struct rb_tree*, struct rb_node*);


/* To be defined by user of this library.*/

extern bool LESS_THAN(char* , char*);

extern bool NOT_EQUAL(char*, char*);

extern void set(struct rb_tree*, char*, char*);

extern bool delete(struct rb_tree*, char*);

extern bool is_member(struct rb_tree*, char*);

extern void rb_free(struct rb_node*);
