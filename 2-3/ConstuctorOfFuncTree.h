#pragma once


class ConstuctorOfFuncTree
{
public:
	ConstuctorOfFuncTree();
	~ConstuctorOfFuncTree();

	

	void Begin(char*);
	void OpenBracket();
	void CloseBracket();
	void addFunction(char*);

	void PrintTreeInConsole();
	void PrintReqInConsole();

private:
	FunctionTree tree;

	std::vector<FunctionNode*> way;
	int countWay;
	FunctionNode* current;
	
};

