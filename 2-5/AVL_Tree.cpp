#include <Windows.h>
#include <vector>

#include "UserMessage.h"
#include "AVL_Tree.h"

///////////// AVL NODE

AVL_Node::AVL_Node(UserMessage* msg) : data(msg), left(nullptr), right(nullptr), height(1)
{}

AVL_Node::~AVL_Node()
{}

///////////// AVL TREE

AVL_Tree::AVL_Tree() : root(nullptr) {}

AVL_Tree::~AVL_Tree() 
{
	DeleteTree(root);
}

unsigned char AVL_Tree::height(AVL_Node* p)
{
	return p ? p->height : 0;
}

int AVL_Tree::bfactor(AVL_Node* p)
{
	return height(p->right) - height(p->left);
}

void AVL_Tree::fixheight(AVL_Node* p)
{
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

AVL_Node* AVL_Tree::rotateright(AVL_Node* p) // правый поворот вокруг p
{
	AVL_Node* q = p->left;
	p->left = q->right;
	q->right = p;
	fixheight(p);
	fixheight(q);
	return q;
}

AVL_Node* AVL_Tree::rotateleft(AVL_Node* q) // левый поворот вокруг q
{
	AVL_Node* p = q->right;
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);
	return p;
}

AVL_Node* AVL_Tree::balance(AVL_Node* p) // балансировка узла p
{
	fixheight(p);
	if (bfactor(p) == 2)
	{
		if (bfactor(p->right) < 0)
			p->right = rotateright(p->right);
		return rotateleft(p);
	}
	if (bfactor(p) == -2)
	{
		if (bfactor(p->left) > 0)
			p->left = rotateleft(p->left);
		return rotateright(p);
	}
	return p; // балансировка не нужна
}

void AVL_Tree::insert(UserMessage* k)
{
	root = insert_R(root, k);
}

AVL_Node* AVL_Tree::insert_R(AVL_Node* p, UserMessage* k) // вставка ключа k в дерево с корнем p
{
	if (!p) return new AVL_Node(k);

	if (*k < *p->data)
		p->left = insert_R(p->left, k);
	else
		p->right = insert_R(p->right, k);
	return balance(p);
}

AVL_Node* AVL_Tree::find(UserMessage* k)
{
	AVL_Node* current = root;
	while (current != nullptr)
		if (*k == *current->data)
			return (current);
		else
			current = (*k < *current->data) ?
			current->left : current->right;
	return(0);
}

AVL_Node* AVL_Tree::findmin(AVL_Node* p) // поиск узла с минимальным ключом в дереве p 
{
	return p->left ? findmin(p->left) : p;
}

AVL_Node* AVL_Tree::removemin(AVL_Node* p) // удаление узла с минимальным ключом из дерева p
{
	if (p->left == 0)
		return p->right;
	p->left = removemin(p->left);
	return balance(p);
}

AVL_Node* AVL_Tree::remove(AVL_Node* p, UserMessage* k) // удаление ключа k из дерева p
{
	if (!p) return 0;
	if (*k < *p->data)
		p->left = remove(p->left, k);
	else if (*k > *p->data)
		p->right = remove(p->right, k);
	else //  k == p->key 
	{
		AVL_Node* q = p->left;
		AVL_Node* r = p->right;
		delete p;
		if (!r) return q;
		AVL_Node* min = findmin(r);
		min->right = removemin(r);
		min->left = q;
		return balance(min);
	}
	return balance(p);
}

void AVL_Tree::removeMsg(UserMessage* k)
{
	root = remove(root, k);
}

void AVL_Tree::DeleteTree(AVL_Node* node)
{
	if (node != nullptr) {
		DeleteTree(node->left);
		DeleteTree(node->right);
		delete node;
	}
}



//////////   MORE

void AVL_Tree::findMessagesByName(std::vector<UserMessage*>& messages, char* name)
{
	messages.clear();

	findMessagesByName_R(root, messages, name);
}

void AVL_Tree::findMessagesByName_R(AVL_Node* node, std::vector<UserMessage*>& messages, char* name)
{
	if (node != nullptr) {
		if (!strcmp(name, node->data->userName))
		{
			messages.push_back(node->data);
		}
		findMessagesByName_R(node->left, messages, name);
		findMessagesByName_R(node->right, messages, name);
	}
}

void AVL_Tree::findMessagesByTime(std::vector<UserMessage*>& messages, SYSTEMTIME& start, SYSTEMTIME& end)
{
	messages.clear();

	findMessagesByTime_R(root, messages, start, end);
}

void AVL_Tree::findMessagesByTime_R(AVL_Node* node, std::vector<UserMessage*>& messages, SYSTEMTIME& start, SYSTEMTIME& end)
{
	if (node != nullptr) {
		if (node->data->time >= start && node->data->time <= end)
		{
			messages.push_back(node->data);
		}
		findMessagesByTime_R(node->left, messages, start, end);
		findMessagesByTime_R(node->right, messages, start, end);
	}
}