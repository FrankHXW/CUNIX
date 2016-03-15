#include "stdio.h"
#include "btree.h"

int main(void)
{
	btree root;
	init_btree(&btree);
	root=createBTree(&root);	
	preOrder(root);
	return 0;
}
