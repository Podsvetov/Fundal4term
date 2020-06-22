#pragma once

/*class Key
{
public:
	Key(char*, SYSTEMTIME*);

	char* name;
	SYSTEMTIME* time;
	bool operator<(const Key& other);
	bool operator==(const Key& other);

private:

};*/

class AVL_Node
{
public:
	AVL_Node(UserMessage*);
	~AVL_Node();

	unsigned char height;
	AVL_Node* left;
	AVL_Node* right;
	UserMessage* data;

private:
	AVL_Node();
};



class AVL_Tree
{
public:
	AVL_Tree();
	~AVL_Tree();

	void insert(UserMessage* k);
	AVL_Node* find(UserMessage* k);
	void removeMsg(UserMessage* k);

	AVL_Node* root;

	void findMessagesByName(std::vector<UserMessage*>&, char*);
	void findMessagesByTime(std::vector<UserMessage*>&, SYSTEMTIME&, SYSTEMTIME&);

private:
	unsigned char height(AVL_Node* p);
	int bfactor(AVL_Node* p);
	void fixheight(AVL_Node* p);
	AVL_Node* rotateright(AVL_Node* p);
	AVL_Node* rotateleft(AVL_Node* q);
	AVL_Node* balance(AVL_Node* p);
	AVL_Node* findmin(AVL_Node* p);
	AVL_Node* removemin(AVL_Node* p);
	AVL_Node* remove(AVL_Node* p, UserMessage* k);
	

	AVL_Node* insert_R(AVL_Node* p, UserMessage* k);

	void DeleteTree(AVL_Node* node);

	////	MORE

	void findMessagesByName_R(AVL_Node* node, std::vector<UserMessage*>&, char*);
	void findMessagesByTime_R(AVL_Node* node, std::vector<UserMessage*>&, SYSTEMTIME&, SYSTEMTIME&);
};