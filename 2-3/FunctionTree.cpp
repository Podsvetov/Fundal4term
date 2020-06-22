#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>

#include "FunctionTree.h"



////////////////////////////////////////////////
//////////		FunctionNode		////////////
////////////////////////////////////////////////

FunctionNode::FunctionNode(char* _key) : key(nullptr), data(nullptr), count(0)
{
	if (_key)
	{
		key = new char[strlen(_key) + 1];
		strcpy(key, _key);
	}
}

FunctionNode::~FunctionNode() 
{
	delete[] key;
}



////////////////////////////////////////////////
//////////		FunctionTree		////////////
////////////////////////////////////////////////

FunctionTree::FunctionTree() : root(nullptr)
{}

FunctionTree::~FunctionTree() 
{
	DeleteTree(root);
}

void FunctionTree::SetRoot(FunctionNode* c)
{
	if(!root)
		root = c;
}

void FunctionTree::addTreeToNode(FunctionNode* node, FunctionNode* new_node)
{
	FunctionNode** copy = new FunctionNode*[node->count + 1];
	int i;
	for (i = 0; i < node->count; ++i)
	{
		copy[i] = node->data[i];
	}
	copy[i] = new_node;

	delete[] node->data;

	node->data = copy;

	node->count++;
}

void FunctionTree::PrintTree()
{
	PrintTree_R(root);
}

void FunctionTree::PrintTree_R(FunctionNode* node, int x)
{
	for (int i = 0; i < x; ++i)
	{
		printf("  ");
	}

	printf("%s\n", node->key);

	for (int i = 0; i < node->count; ++i)
	{
		PrintTree_R(*(node->data + i), x + 1);
	}
}

void FunctionTree::DeleteTree(FunctionNode* node)
{
	if (node)
	{
		for (int i = 0; i < node->count; ++i)
		{
			DeleteTree(*(node->data + i));
		}
		delete node;
	}
}


void FunctionTree::PrintRec()
{
	std::vector<std::vector<char*>> recs;

	PrintRec_R(root, recs, true);

	for (int i = 0; i < recs.size(); ++i)
	{
		if (recs[i].size())
		{
			printf("I found a recurcive function (%s): ", recs[i][0]);
			for (int j = 0; j < recs[i].size(); ++j)
			{
				printf("%s", recs[i][j]);

				if (j != recs[i].size() - 1)
				{
					printf(" -> ");
				}
			}
			printf("\n");
		}
	}
}

void FunctionTree::PrintRec_R(FunctionNode* node, std::vector<std::vector<char*>>& recs, bool skip)
{
	if (!skip)
	{
		std::vector<char*> rec = IfRec(node);
		if (rec.size() > 0)
		{
			recs.push_back(rec);
		}
	}

	for (int i = 0; i < node->count; ++i)
	{
		FunctionNode* next = *(node->data + i);
		skip = false;

		for (int j = 0; j < recs.size(); ++j)
		{
			if (!strcmp(recs[j][0], next->key))
				skip = true;
		}

		PrintRec_R(next, recs, skip);
	}
}


std::vector<char*> FunctionTree::IfRec(FunctionNode* node)
{
	bool ok = false;
	std::vector<char*> rec;

	IfRec_R(node, rec, ok);

	return rec;
}

void FunctionTree::IfRec_R(FunctionNode* node, std::vector<char*>& rec, bool& ok)
{
	if (rec.size() > 0 && !strcmp(node->key, rec[0]))
	{
		char* name = new char[strlen(node->key) + 1];
		strcpy(name, node->key);
		rec.push_back(name);
		ok = true;
		return;
	}

	for (int i = 0; i < node->count; ++i)
	{
		char* name = new char[strlen(node->key) + 1];
		strcpy(name, node->key);
		rec.push_back(name);

		IfRec_R(*(node->data + i), rec, ok);
		
		if (ok)
			break;

		delete[] rec[rec.size() - 1];
		rec.pop_back();
	}
}