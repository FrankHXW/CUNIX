#include "stdio.h"
#include "stdlib.h"
#include "btree.h"

//初始化btree root
void init_btree(btree *ptr_root)
{
	ptr_root->lchild=ptr_root;
	ptr_root->rchild=ptr_root;
}

//先序创建二叉树
btree *createBTree(btree *ptr_root)
{
	char ch=getchar();
	if(ch=='#'){
		ptr_root=NULL;
		return ptr_root;
	}
	else{
		my_btree *node=(my_btree *)malloc(sizeof(my_btree));
		node->data=ch;
		ptr_root->lchild=createBTree((node->root).lchild);
		ptr_root->rchild=createBTree((node->root).rchild);
		return ptr_root;
	}
}

//清空二叉树
void clearBTree(btree *root)
{


}

//先序遍历二叉树
void preOrder(btree *ptr_root)
{	
	if(ptr_root!=NULL){
		my_btree *node=btree_entry(ptr_root,my_btree,root);
		printf("%c ",node->data);
		preOrder(ptr_root->lchild);
		preOrder(ptr_root->rchild); 
	}	
}

//中序遍历二叉树
void midOrder(btree root)
{


}

//后序遍历二叉树
void postOrder(btree root);

//输出叶子节点
void displayLeaf(btree root);

//左节点插入
void insertLeftNode(btree root,char ch);

//右节点插入
void insertRightNode(btree root,char ch);

//删除左子树
void deleteLeftTree(btree root);

//删除右子树
void deleteRightTree(btree root);
