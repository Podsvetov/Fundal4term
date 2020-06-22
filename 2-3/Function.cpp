#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include<iostream>
#include <strstream>

#include "Function.h"

Function::Function(char* _name) : name(nullptr), haveBody(false)
{
	if (_name)
	{
		name = new char[strlen(_name) + 1];
		strcpy(name, _name);
	}
}

Function::Function(const Function& other) : name(nullptr), haveBody(other.haveBody)
{
	if (other.name)
	{
		name = new char[strlen(other.name) + 1];
		strcpy(name, other.name);
	}

	int argSize = other.args.size();
	int doingSize = other.doing.size();

	for (int i = 0; i < argSize; ++i)
	{
		char* str = new char[strlen(other.args[i]) + 1];
		strcpy(str, other.args[i]);
		args.push_back(str);
	}

	for (int i = 0; i < doingSize; ++i)
	{
		char* str = new char[strlen(other.doing[i]) + 1];
		strcpy(str, other.doing[i]);
		doing.push_back(str);
	}
}

Function::~Function()
{
	delete[] name;

	for (int i = 0; i < args.size(); ++i)
	{
		delete[] args[i];
	}

	for (int i = 0; i < doing.size(); ++i)
	{
		delete[] doing[i];
	}
}


const char* Function::GetName()const
{
	return name;
}

int Function::GetCountArgs() const
{
	return args.size();
}

std::vector<char*>& Function::GetArgs()
{
	return args;
}

std::vector<char*>& Function::GetDoing()
{
	return doing;
}

bool Function::GetHaveBody()
{
	return haveBody;
}


void Function::addArg(char* s)
{
	char* str = new char[strlen(s) + 1];
	strcpy(str, s);
	args.push_back(str);
}

void Function::addDoing(char* s)
{
	char* str = new char[strlen(s) + 1];
	strcpy(str, s);
	doing.push_back(str);

	if (!haveBody)
		haveBody = true;
}

void Function::PrintAbout()
{
	printf("====================\n");

	printf("void ");

	printf("%s(", name);

	for (int i = 0; i < args.size(); ++i)
	{
		printf("%s ", args[i]);
	}

	if (haveBody)
	{
		printf(")\n{\n\t");

		for (int i = 0; i < doing.size(); ++i)
		{
			if (isEndOfCommand(doing[i]))
			{
				printf("%s\n\t", doing[i]);
			}
			else
			{
				printf("%s ", doing[i]);
			}
		}

		printf("\n}\n====================\n");
	}
	else
	{
		printf(");\nIt is a function without body!\n");
		printf("====================\n");
	}
}	

////////////






////////////
//	Типы
bool isInt(char* s)
{
	return !strcmp(s, "int");
}

bool isFloat(char* s)
{
	return !strcmp(s, "float");
}

bool isVoid(char* s)
{
	return !strcmp(s, "void");
}

bool isType(char* s)
{
	return isInt(s) || isFloat(s) || isVoid(s);
}

////////////
//	Специальные символы для строк
bool isEndOfCommand(char* s)
{
	return !strcmp(s, ";");
}

bool isOpenRound(char* s)
{
	return !strcmp(s, "(");
}

bool isCloseRound(char* s)
{
	return !strcmp(s, ")");
}

bool isOpenFigure(char* s)
{
	return !strcmp(s, "{");
}

bool isCloseFigure(char* s)
{
	return !strcmp(s, "}");
}

bool isComma(char* s)
{
	return !strcmp(s, ",");
}

bool isSpecialSymbol(char* s)
{
	return isEndOfCommand(s) || isOpenRound(s) || isCloseRound(s) || isOpenFigure(s) || isCloseFigure(s) ||
		isComma(s);
}

////////////
//	Специальные символы для чаров
bool isEndOfCommand(char s)
{
	return (s == ';');
}

bool isOpenRound(char s)
{
	return (s == '(');
}

bool isCloseRound(char s)
{
	return (s == ')');
}

bool isOpenFigure(char s)
{
	return (s == '{');
}

bool isCloseFigure(char s)
{
	return (s == '}');
}

bool isComma(char s)
{
	return (s == ',');
}

bool isSpecialSymbol(char s)
{
	return isEndOfCommand(s) || isOpenRound(s) || isCloseRound(s) || isOpenFigure(s) || isCloseFigure(s) || 
		isComma(s);
}

////////////
//	Проверка на число
bool isNum(char* s)
{
	int size = strlen(s);
	for (int i = 0; i < size; ++i)
	{
		if (!isdigit(s[i])) return false;
	}

	return true;
}

bool isNumFloat(char* s)
{
	bool dot = false;

	int size = strlen(s);
	for (int i = 0; i < size; ++i)
	{
		if (dot)
		{
			if (!isdigit(s[i])) return false;
		}
		else
		{
			if (!isdigit(s[i]) && s[i] != '.') return false;
			if (s[i] == '.') dot = true;
		}
	}

	return true;
}

float TranslateNum(char* s)
{
	float f; 
	std::strstream stream;
	stream  << s;
	stream >> f;
	return f;
}