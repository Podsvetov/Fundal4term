#pragma once

#define SIZE_WORD	50

class DictWord
{
public:
	explicit DictWord(char*);
	~DictWord();

	char word[SIZE_WORD];

private:
	DictWord();
};

bool Equal(char*, char*);

