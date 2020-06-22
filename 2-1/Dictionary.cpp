#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>

#include "DictWord.h"
#include "Dictionary.h"

Dictionary::Dictionary()
{}

Dictionary::~Dictionary()
{
	int size = dict.size();

	for (int i = 0; i < size; ++i)
	{
		delete dict[i];
	}
}

bool IsDigit(char c)
{
	return c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '0';
}

//	PUBLIC

bool Dictionary::GetDict(const char* file)
{
	FILE* f;

	f = fopen(file, "r");
	if (!f)
	{
		return false;
	}

	char buff[SIZE_WORD];
	char* ptr = 0;

	while (!feof(f))
	{
		char c = ' ';
		c = getc(f);
		ptr = buff;
		bool end = false;
		while (!feof(f) && c != '\n')
		{
			if (c != ' ' && !IsDigit(c))
			{
				*ptr = c;
				++ptr;
			}
			c = getc(f);
		}
		*ptr = '\0';

		dict.push_back(new DictWord(buff));
	}

	return true;
}

int Dictionary::FindWord(char* word)
{
	int size = dict.size();

	for (int i = 0; i < size; ++i)
	{
		if (!strcmp(word, dict[i]->word))
		{
			return i;
		}
	}

	return -1;
}

DictWord* Dictionary::GetById(int i)
{
	if(i >= 0 && i < dict.size())
		return dict[i];
	return nullptr;
}