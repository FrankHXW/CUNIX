#include "stdio.h"
#include "stdlib.h"
#include "btree.h"

//初始化btree root
void init_btree(struct btree *ptr_root)
{
	ptr_root->lchild=ptr_root;
	ptr_root->rchild=ptr_root;
}

//先序创建二叉树
struct btree *createBTree(struct btree *ptr_root)
{
	char ch=getchar();
	if(ch=='#'){
		ptr_root=NULL;
		printf("input is #\n");
		return ptr_root;
	}
	else{
		struct 	my_btree *node=(struct my_btree *)malloc(sizeof(struct my_btree));
		node->data=ch;
		printf("input %c\n",ch);
		ptr_root->lchild=createBTree((node->root).lchild);
		ptr_root->rchild=createBTree((node->root).rchild);
		return ptr_root;
	}
}

//清空二叉树
void clearBTree(struct btree *root)
{


}

//先序遍历二叉树
void preOrder(struct btree *ptr_root)
{	
	if(ptr_root!=NULL){
		struct	my_btree *node=btree_entry(ptr_root,struct my_btree,root);
		printf("%c ",node->data);
		preOrder(ptr_root->lchild);
		preOrder(ptr_root->rchild); 
	}
	else {
		printf("ptr_root is null");
	}	
}

//中序遍历二叉树
void midOrder(struct btree *ptr_root)
{


}

//后序遍历二叉树
void postOrder(struct btree root);

//输出叶子节点
void displayLeaf(struct btree root);

//左节点插入
void insertLeftNode(struct btree root,char ch);

//右节点插入
void insertRightNode(struct btree root,char ch);

//删除左子树
void deleteLeftTree(struct btree root);

//删除右子树
void deleteRightTree(struct btree root);
