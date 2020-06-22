#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include "DictWord.h"

DictWord::DictWord(char* _word)
{
	if(_word != nullptr && strlen(_word) < SIZE_WORD)
		strcpy(word, _word);
	else
		strcpy(word, "#error");
}

DictWord::~DictWord()
{

}

bool Equal(char* first, char* second)
{
	if (!first || !second)
		return false;

	char g[] = "àîýåèûó¸þÿ";
	int gSize = strlen(g);

	int fCount = 0, sCount = 0;
	int fSize = strlen(first);
	int sSize = strlen(second);

	for (int i = 0; i < fSize; ++i)
	{
		for(int j = 0; j < gSize; ++j)
			if(first[i] == g[j])
				++fCount;
	}

	for (int i = 0; i < sSize; ++i)
	{
		for (int j = 0; j < gSize; ++j)
			if (second[i] == g[j])
				++sCount;
	}

	return fCount == sCount;
}