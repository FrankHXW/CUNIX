#ifndef _BTREE_H_
#define _BTREE_H_

#define offsetof(type,member)	\
	((size_t) &((type *)0)->member)

#define container_of(ptr,type,member) ({				\
	const typeof(((type *)0)->member) *__mptr=(ptr);		\
	(type *) ( (char *)__mptr-offsetof(type,member) );})



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
struct btree *createBTree(void);

//清空二叉树
void clearBTree(const struct btree *ptr_root);

//先序遍历二叉树
void preOrder(const struct btree *ptr_root);

//中序遍历二叉树
void midOrder(const struct btree *ptr_root);

//后序遍历二叉树
void postOrder(const struct btree *ptr_root);

//输出叶子节点
void displayLeaf(const struct btree *ptr_root);

//求节点总数
int countAllNodes(const struct btree *ptr_root);

//求树的高度
int getHeight(const struct btree *ptr_root);

//求叶子节点的个数
int countLeaf(const struct btree *ptr_root);

//查找节点
struct btree *searchNode(struct btree *ptr_root,char ch);

//左节点插入
void insertLeftNode(struct btree *ptr_root,char ch);

//右节点插入
void insertRightNode(struct btree *ptr_root,char ch);

//删除左子树
void deleteLeftTree(struct btree *ptr_root);

//删除右子树
void deleteRightTree(struct btree *ptr_root);


#endif
