#include "rbtree.h"
#include "unity.h"
#include <string.h>

void test_rbtree_alloc(){
	struct rb_tree *tree = rb_tree_alloc();
	TEST_ASSERT_EQUAL_STRING(tree->root->key, "NIL");
	TEST_ASSERT_EQUAL(tree->root->color, 0);
	TEST_ASSERT_EQUAL(tree->root->parent, NULL);
}

void test_insert_and_retrieve(){
	struct rb_tree *tree = rb_tree_alloc();
	struct rb_node *node;
	char s[2];
	s[1] = '\0';

	for (char c = 'a' ; c <= 'z'; ++c)
	{	
		s[0] = c;
		node = rb_node_alloc_kv(s, s);
		rb_insert(tree, node);
	}

	for (char c = 'a'; c <= 'z'; ++c)
	{	s[0] = c;
		node = rb_search(tree, s);
		TEST_ASSERT_EQUAL_STRING(node->key, s);
		TEST_ASSERT_EQUAL_STRING(node->data, s);
	}
}


void test_check_ordering(){
	struct rb_tree *tree = rb_tree_alloc();
	struct rb_node *node;
	char s[2];
	s[1] = '\0';

	for(char c = 'a'; c <= 'z'; ++c){
		s[0] = c;
		node = rb_node_alloc_kv(s, s);
		rb_insert(tree, node);
	}
	s[0] = 'a';
	for(char c = 'b'; c <= 'z'; ++c){
		node = rb_search(tree, s);
		node = tree_successor(node);
		s[0] = c;
		TEST_ASSERT_EQUAL_STRING(node->key, s);
	}
}


void test_insert_and_delete(){
	struct rb_tree *tree = rb_tree_alloc();
	struct rb_node *node;
	char s[2];
	s[1] = '\0';
	for(char c = 'a'; c <= 'z'; ++c){
		s[0] = c;
		node = rb_node_alloc_kv(s, s);
		rb_insert(tree, node);
	}

	char* delete_set[] = {"a", "h", "i", "t", "u", "n"};
	for (int i = 0; i < 6; i++)
	{
		if (delete(tree, delete_set[i])){
			node = rb_search(tree, delete_set[i]);
			TEST_ASSERT_EQUAL(node, NULL);
		}
	}
}

void test_a_million_items(){
	struct rb_tree *tree = rb_tree_alloc();
	struct rb_node *node;
	char key[10];

	for(int i = 0; i < 1000000; i++){
		sprintf(key, "%d", i);
		node = rb_node_alloc_kv(key, key);
		rb_insert(tree, node);
	}


	for(int i = 0; i < 1000000; i++){
	        sprintf(key, "%d", i);
		node = rb_search(tree, key);
		TEST_ASSERT_EQUAL_STRING(node->key, key);
		TEST_ASSERT_EQUAL_STRING(node->data, key);
	}

	for (int i = 0; i < 1000000; i++){
	  memset(key, 0, sizeof(key));
	  sprintf(key, "%d", i);
	  TEST_ASSERT_EQUAL(delete(tree, key), true);
	}
}


int main(int argc, char const *argv[])
{
	UNITY_BEGIN();
	RUN_TEST(test_rbtree_alloc);
	RUN_TEST(test_insert_and_retrieve);
	RUN_TEST(test_check_ordering);
	RUN_TEST(test_insert_and_delete);
	RUN_TEST(test_a_million_items);
	UNITY_END();

	return 0;
}
