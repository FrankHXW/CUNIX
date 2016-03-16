#include "stdio.h"
#include "stdlib.h"
#include "btree.h"

int main(void)
{
	struct btree *ptr_root=malloc(sizeof(struct btree));
	init_btree(ptr_root);
	createBTree(ptr_root);	
	preOrder(ptr_root);
	return 0;
}
