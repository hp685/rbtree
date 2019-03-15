#include "rbtree.h"
#include "unity.h"


void test_rbtree_alloc(){
	struct rb_tree *tree = rb_tree_alloc();
	TEST_ASSERT_EQUAL(tree->root->key, "NIL");
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



int main(int argc, char const *argv[])
{
	UNITY_BEGIN();
	RUN_TEST(test_rbtree_alloc);
	RUN_TEST(test_insert_and_retrieve);
	UNITY_END();

	return 0;
}