#pragma once

#include <vector>

enum DoingCase
{
	NO,				//новая комманда
	TYPE,			//найден тип
	FUNCNAME,		//найденно имя функции
	FUNCOPEN,		//начало считывания аргуметов фунции
	RETURNEND,		//
	WAITEND,		//
	STOP
};

class FunctionStorage
{
public:
	FunctionStorage();
	~FunctionStorage();

	void addFunction(Function&);
	void PrintAllFunctions();
	
	int findFunctionByName(char* name);
	Function* returnFunctionByName(char* name);

	int Do(const char* name, int countArgs, ConstuctorOfFuncTree& treeConstuctor);

	int TryDoFun(int funcId, std::vector<char*>& doing, int& i, ConstuctorOfFuncTree& treeConstuctor);

private:
	std::vector<Function> functions;
};

