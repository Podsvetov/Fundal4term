#pragma once

#include <vector>
#include <ctype.h>

class Function
{
public:
	Function(char* _name);
	Function(const Function&);
	~Function();

	const char* GetName() const;
	int GetCountArgs() const;
	std::vector<char*>& GetArgs();
	std::vector<char*>& GetDoing();
	bool GetHaveBody();

	void addArg(char*);
	void addDoing(char*);
	void PrintAbout();

private:
	bool haveBody;
	char* name;
	std::vector<char*> args;
	std::vector<char*> doing;
};

////////////
//	Типы
bool isInt(char* s);// int
bool isFloat(char* s);// float
bool isVoid(char* s);// void
bool isType(char* s);

////////////
//	Специальные символы для строк
bool isEndOfCommand(char* s);// ;
bool isOpenRound(char* s);// (
bool isCloseRound(char* s);// )
bool isOpenFigure(char* s);// {
bool isCloseFigure(char* s);// }
bool isComma(char* s);// ,
bool isSpecialSymbol(char* s);

////////////
//	Специальные символы для чаров
bool isEndOfCommand(char s);// ;
bool isOpenRound(char s);// (
bool isCloseRound(char s);// )
bool isOpenFigure(char s);// {
bool isCloseFigure(char s);// }
bool isComma(char s);// ,
bool isSpecialSymbol(char s);

////////////
//	Проверка на число
bool isNum(char* s);
bool isNumFloat(char* s);
float TranslateNum(char* s);