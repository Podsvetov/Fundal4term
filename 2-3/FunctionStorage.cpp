#define _CRT_SECURE_NO_WARNINGS

#include <vector>

#include <iostream>
#include <strstream>
#include <time.h>

#include"FunctionTree.h"
#include "ConstuctorOfFuncTree.h"
#include "Function.h"
#include "FunctionStorage.h"


FunctionStorage::FunctionStorage()
{

}

FunctionStorage::~FunctionStorage()
{
}

void FunctionStorage::addFunction(Function& function)
{
	functions.push_back(function);
}

void FunctionStorage::PrintAllFunctions()
{
	for (int i = 0; i < functions.size(); ++i)
	{
		functions[i].PrintAbout();
	}
}

int FunctionStorage::findFunctionByName(char* name)
{
	int size = functions.size();
	for (int i = 0; i < size; ++i)
	{
		if (!strcmp(functions[i].GetName(), name))
			return i;
	}

	return -1;
}

Function* FunctionStorage::returnFunctionByName(char* name)
{
	int size = functions.size();
	for (int i = 0; i < size; ++i)
	{
		if (!strcmp(functions[i].GetName(), name))
			return &functions[i];
	}

	return nullptr;
}

int FunctionStorage::Do(const char* name, int countArgs, ConstuctorOfFuncTree& treeConstuctor)
{
	/*
	0 - ��
	1 - ������� main �� ��������
	2 - ����� �� ������ �� �������� �����
	3 - ������������ ��� ����������
	4 - ������ � ���������� ����������
	6 - �� �������� ( ����� ����� �������
	8 - � �������� ������� ��� ������� ������������ ������
	9 - ������ ���������� ���������� �������
	10 - ����� ������� ��� ����
	11 - ���������� ;

	99 - ��� �� �������
	*/

	Function* func = nullptr;
	int size = functions.size();
	for (int i = 0; i < size; ++i)
	{
		if (!strcmp(functions[i].GetName(), name) && functions[i].GetCountArgs() == countArgs)
		{
			func = &functions[i];
			break;
		}
	}

	if (!func)
		return 1;//������� �� ��������

	std::vector<char*> args = func->GetArgs();
	std::vector<char*> doing = func->GetDoing();
	int argsCount = args.size(); int currentArg = 0;
	int doingCount = doing.size();

	DoingCase doingCase = NO;

	int funcId = -1;


	for (int i = 0; i < argsCount; ++i)//��������� ����������
	{
		switch (doingCase)
		{
		case NO:
			if (isType(args[i]))
			{
				doingCase = DoingCase::TYPE;
			}
			else
			{
				return 2;//����� �� ������ �� �������� �����
			}
			break;


		case TYPE:
			if (isSpecialSymbol(args[i]) || isType(args[i]))
				return 3;//������������ ��� ����������

			doingCase = DoingCase::NO;
			break;
		}
	}//��������� ����������

	
	
	if (doingCase != DoingCase::NO)
	{
		return 4;//������ � ���������� ����������
	}




	for (int i = 0; i < doingCount; ++i)//������
	{
		switch (doingCase)
		{
		case NO://���� ������ ����� ��������

			if (!strcmp(doing[i], "return"))
			{
				doingCase = DoingCase::RETURNEND;
			}
			else if (!strcmp(doing[i], "randStop"))
			{
				//doingCase = DoingCase::STOP;

				doingCase = DoingCase::WAITEND;
				strcpy(doing[i], "return");
			}
			else if (findFunctionByName(doing[i]) != -1)
			{
				funcId = findFunctionByName(doing[i]);
				int err = 0;

				treeConstuctor.addFunction(doing[i]);
				treeConstuctor.OpenBracket();

				err = TryDoFun(funcId, doing, i, treeConstuctor);

				treeConstuctor.CloseBracket();

				if (err != 0)
					return err;
				
				//--i;
				doingCase = WAITEND;
			}
			else
			{
				//
			}
			break;


		case WAITEND:
			if (!isEndOfCommand(doing[i]))
			{
				return 11;
			}
			else
			{
				doingCase = NO;
			}
			break;


		case RETURNEND:
			if (isEndOfCommand(doing[i]))
			{
				return 0;
			}
			else
			{
				return 11;//����������;
			}
			break;


		case STOP:
			if (isEndOfCommand(doing[i]))
			{
				int r = rand() % 10;
				if (!r)
				{
					return 0;
				}
				else
				{
					doingCase = NO;
				}
			}
			else
			{
				return 11;//����������;
			}
			break;

		}


	}//������

	if (doingCase != DoingCase::NO)
	{
		//return 4;//������ � ���������� ����������
	}

	return 0;
}



int FunctionStorage::TryDoFun(int funcId, std::vector<char*>& doing, int& i, ConstuctorOfFuncTree& treeConstuctor)
{
	/*
	0 - ��
	1 - ������� main �� ��������
	2 - ����� �� ������ �� �������� �����
	3 - ������������ ��� ����������
	4 - ������ � ���������� ����������
	5 - ������������ ��� ����������
	6 - �� �������� ( ����� ����� �������
	7 - ������ func(1,)
	8 - � �������� ������� ��� ������� ������������ ������
	9 - ������ ���������� ���������� �������
	10 - ���������� ������� ����� ����������� �������
	11 - ���������� ;
	12 - ����� �� ������ �� �������� �����������
	13 - ������� �� 0

	99 - ��� �� �������
	*/

	Function* func = &functions[funcId];

	int doingCount = doing.size();

	DoingCase doingCase = FUNCNAME;

	int findArgs = 0;

	++i;
	
	for (; i < doingCount; ++i)//������
	{
		switch (doingCase)
		{

		case FUNCNAME://���� �������� ��� �������
			if (isOpenRound(doing[i]))
				doingCase = DoingCase::FUNCOPEN;
			else
				return 6;//�� �������� ( ����� ����� �������
			break;

		case FUNCOPEN:
			if (isCloseRound(doing[i]))
			{
				if (findArgs * 2 == functions[funcId].GetCountArgs())
				{
					if (!functions[funcId].GetHaveBody())
						return 10;

					//printf("Function %s started\n", functions[funcId].GetName());
					return Do(functions[funcId].GetName(), functions[funcId].GetCountArgs(), treeConstuctor);
				}
				else
				{
					return 9;//������ func(1,)
				}
			}
			else if (!isSpecialSymbol(doing[i]) && !isType(doing[i]))
			{
				findArgs++;
			}
			else if(!isComma(doing[i]))
			{
				return 8;//� �������� ������� ��� ������� ������������ ������
			}
			break;
		}

	}//������

	return 99;
}
