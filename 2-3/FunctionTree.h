#pragma once




class FunctionNode
{
public:
	FunctionNode(char*);
	~FunctionNode();

	char* key;
	FunctionNode** data;
	int count;

private:
};




class FunctionTree
{
public:
	FunctionTree();
	~FunctionTree();

	void addTreeToNode(FunctionNode* node, FunctionNode* new_node);
	void PrintTree();
	void SetRoot(FunctionNode*);

	void PrintRec();

private:
	FunctionNode* root;

	void PrintTree_R(FunctionNode* node, int x = 0);
	void DeleteTree(FunctionNode* node);

	void PrintRec_R(FunctionNode* node, std::vector<std::vector<char*>>& recs, bool skip);

	std::vector<char*> IfRec(FunctionNode* node);
	void IfRec_R(FunctionNode* node, std::vector<char*>& rec, bool& ok);
};

