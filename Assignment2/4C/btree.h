#ifndef _BTREE_H_
#define _BTREE_H_


#define btree_entry(ptr,type,member) container_of(ptr,type,member)


struct btree{
	struct  btree *lchild,*rchild;
};

struct my_btree{
	char data;
	struct btree root; 
};

//初始化btree root
void init_btree(struct btree *ptr_root);

//先序创建二叉树
struct btree *createBTree(struct btree *ptr_root);

//清空二叉树
void clearBTree(struct btree *ptr_root);

//先序遍历二叉树
void preOrder(struct btree *ptr_root);

//中序遍历二叉树
void midOrder(struct btree *ptr_root);

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


#endif
