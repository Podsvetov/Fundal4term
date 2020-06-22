#include <vector>
#include<iostream>

#include"FunctionTree.h"
#include "ConstuctorOfFuncTree.h"

ConstuctorOfFuncTree::ConstuctorOfFuncTree() : countWay(0), current(nullptr)
{}

ConstuctorOfFuncTree::~ConstuctorOfFuncTree() {}

void ConstuctorOfFuncTree::Begin(char* c)
{
	current = new FunctionNode(c);
	tree.SetRoot(current);
}

void ConstuctorOfFuncTree::OpenBracket()
{
	way.push_back(current);
	current = *(current->data + current->count - 1);
	countWay++;
}

void ConstuctorOfFuncTree::CloseBracket()
{
	countWay--;
	current = way[countWay];
	way.pop_back();
}

void ConstuctorOfFuncTree::addFunction(char* c)
{
	tree.addTreeToNode(current, new FunctionNode(c));
}

void ConstuctorOfFuncTree::PrintTreeInConsole()
{
	tree.PrintTree();
}

void ConstuctorOfFuncTree::PrintReqInConsole()
{
	tree.PrintRec();
}