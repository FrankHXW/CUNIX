#include "stdio.h"
#include "stdlib.h"
#include "btree.h"


int main(void)
{
	printf("following is the test of binary tree:\n");
	while(1){
		struct btree *ptr_root=malloc(sizeof(struct btree));
		init_btree(ptr_root);
		printf("\nplease input binary tree in pre-order:\n");
		ptr_root=createBTree();	
		
		printf("\n\nprint btree in pre-order:\n");
		preOrder(ptr_root);
		
		printf("\n\nprint btree in mid-order:\n");
		midOrder(ptr_root);
		
		printf("\n\nprint btree in post-order:\n");
		postOrder(ptr_root);

		printf("\n\nthis btree's nodes' amount: \n%d",countAllNodes(ptr_root));		
		
		printf("\n\nthis btree's height: \n%d",getHeight(ptr_root));

		printf("\n\nthis btree's leaf's amount:\n%d",countLeaf(ptr_root));
				
		printf("\n\nprint btree's leaf:\n");
		displayLeaf(ptr_root);
		
		printf("\n\nfind 'a' and insert 'x' in left,then display all node in pre-oreder:\n");
		struct btree *foundRoot=searchNode(ptr_root,'a');
		insertLeftNode(foundRoot,'x');
		preOrder(ptr_root);

		printf("\n\nfind 'a' and insert 'y' in left,then display all node in pre-oreder:\n");
		foundRoot=searchNode(ptr_root,'a');
		insertRightNode(foundRoot,'y');
		preOrder(ptr_root);


		printf("\n\ndelete leftTree and display left node in pre-order:\n");
		deleteLeftTree(ptr_root);
		preOrder(ptr_root);

		printf ("\n\ndelete rightTree and display left node in pre-order:\n");
		deleteRightTree(ptr_root);
		preOrder(ptr_root);

		printf("\n\nclear btree!\n");
		clearBTree(ptr_root);
		//clear input buffer
		char ch;
		while((ch=getchar())!='\n'&&ch!=EOF);
	}

	return 0;
}
