#ifndef _BTREE_H_
#define _BTREE_H_


#define btree_entry(ptr,type,member) \
		container_of(ptr,type,member)


typedef struct btree{
	struct  btree *lchild,*rchild;
} btree;

typedef struct my_btree{
	char data;
	struct btree root; 
}my_btree;

//初始化btree root
void init_btree(btree *ptr_root);

//先序创建二叉树
btree *createBTree(btree *ptr_root);

//清空二叉树
void clearBTree(btree *ptr_root);

//先序遍历二叉树
void preOrder(btree *ptr_root);

//中序遍历二叉树
void midOrder(btree *ptr_root);

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


#endif
