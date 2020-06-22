#pragma once

#include "DictWord.h"

class Dictionary
{
public:
	Dictionary();
	~Dictionary();

	bool GetDict(const char*);
	int FindWord(char*);
	DictWord* GetById(int);

private:
	std::vector<DictWord*> dict;
};

