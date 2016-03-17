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
struct btree *createBTree(void)
{
	char ch=getchar();
	if(ch=='#'){
		return NULL;
	}
	else{
		struct 	my_btree *node=(struct my_btree *)malloc(sizeof(struct my_btree));
		node->data=ch;
		(node->root).lchild=createBTree();
		(node->root).rchild=createBTree();
		return &(node->root);
	}
}


//清空二叉树
void clearBTree(const struct btree *ptr_root)
{
	if(ptr_root!=NULL){
		struct btree *pl=ptr_root->lchild;
		struct btree *pr=ptr_root->rchild;
		struct my_btree *node=btree_entry(ptr_root,struct my_btree,root);
		free(node);
		clearBTree(pl);
		clearBTree(pr);
	}
}


//先序遍历二叉树
void preOrder(const struct btree *ptr_root)
{	
	if(ptr_root!=NULL){
		struct	my_btree *node=btree_entry(ptr_root,struct my_btree,root);
		printf("%c ",node->data);
		preOrder(ptr_root->lchild);
		preOrder(ptr_root->rchild); 
	}
}


//中序遍历二叉树
void midOrder(const struct btree *ptr_root)
{
	if(ptr_root!=NULL){
		midOrder(ptr_root->lchild);
		struct	my_btree *node=btree_entry(ptr_root,struct my_btree,root);
		printf("%c ",node->data);
		midOrder(ptr_root->rchild); 
	}
}


//后序遍历二叉树
void postOrder(const struct btree *ptr_root)
{
	if(ptr_root!=NULL){
		postOrder(ptr_root->lchild);
		postOrder(ptr_root->rchild);
		struct	my_btree *node=btree_entry(ptr_root,struct my_btree,root);
		printf("%c ",node->data); 
	}
}


//输出叶子节点
void displayLeaf(const struct btree *ptr_root)
{
	if(ptr_root!=NULL){
		if((ptr_root->lchild==NULL)&&(ptr_root->rchild==NULL)){
			struct my_btree *node=btree_entry(ptr_root,struct my_btree,root);
			printf("%c ",node->data);
		}
		else{
			displayLeaf(ptr_root->lchild);
			displayLeaf(ptr_root->rchild);
		}
	}
}


//求节点总数
int countAllNodes(const struct btree *ptr_root)
{
	if(ptr_root!=0){
		return (countAllNodes(ptr_root->lchild)+countAllNodes(ptr_root->rchild)+1);
	}
	return 0;
}

//求树的高度
int getHeight(const struct btree *ptr_root)
{
	int lchildHeight=0;
	int rchildHeight=0;
	if(ptr_root!=NULL)	{
		lchildHeight=getHeight(ptr_root->lchild);
		rchildHeight=getHeight(ptr_root->rchild);
		return (lchildHeight>rchildHeight?(1+lchildHeight):(1+rchildHeight));
	}
	return 0;
}


//求叶子节点的个数
int countLeaf(const struct btree *ptr_root)
{
	if(ptr_root!=NULL){
		if((ptr_root->lchild==NULL)&&(ptr_root->rchild==NULL)){
			return 1;
		}
		else {
			return countLeaf(ptr_root->lchild)+countLeaf(ptr_root->rchild);
		}
	}
	return 0;
}


//查找节点
struct btree *searchNode(struct btree *ptr_root,char ch)
{
	if(ptr_root!=NULL){
		struct my_btree *node=btree_entry(ptr_root,struct my_btree,root);
		if(node->data==ch){
			return ptr_root;
		}
		else{
			struct btree *tmp=searchNode(ptr_root->lchild,ch);
			if(tmp!=NULL){
				return tmp;
			}
			else{
				return searchNode(ptr_root->rchild,ch);
			}
		}
	}	
	return NULL;
}


//左节点插入
void insertLeftNode(struct btree *ptr_root,char ch)
{
	struct my_btree *node=(struct my_btree *)malloc(sizeof(struct my_btree));
	node->data=ch;
	(node->root).lchild=ptr_root->lchild;
	(node->root).rchild=NULL;
	ptr_root->lchild=&(node->root);
}


//右节点插入
void insertRightNode(struct btree *ptr_root,char ch)
{
	struct my_btree *node=(struct my_btree *)malloc(sizeof(struct my_btree));
	node->data=ch;
	(node->root).lchild=NULL;
	(node->root).rchild=ptr_root->rchild;
	ptr_root->rchild=&(node->root);
}


//删除左子树
void deleteLeftTree(struct btree *ptr_root)
{
	if(ptr_root!=NULL){
		clearBTree(ptr_root->lchild);
		ptr_root->lchild=NULL;
	}
}


//删除右子树
void deleteRightTree(struct btree *ptr_root)
{
	if(ptr_root!=NULL){
		clearBTree(ptr_root->rchild);
		ptr_root->rchild=NULL;
	}
}




















































































































