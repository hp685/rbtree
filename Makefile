CC=gcc
CFLAGS= -I ./unity/src/  -std=c99 -ggdb
TFLAGS= ./unity/src/unity.c

test: test_rbtree
test_rbtree: test_rbtree.c
	$(CC) $(CFLAGS) $(TFLAGS) rbtree.c test_rbtree.c -o test_rb_tree.o
	./test_rb_tree.o
clean:
	rm *.o
