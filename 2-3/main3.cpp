
#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<ctype.h>
#include <vector>
#include <strstream>
#include <time.h>

#include "Function.h"
#include "FunctionTree.h"
#include "ConstuctorOfFuncTree.h"
#include "FunctionStorage.h"


int SizeOf(char* nameOfFile)
{
	FILE* in;
	in = fopen(nameOfFile, "r");
	if (!in)
	{
		printf("I can`t open in.txt\n");
		exit(-1);
	}

	char c;
	int len = 0;

	while (!feof(in))
	{
		c = fgetc(in);
		bool spase = false;

		if (isspace(c))
		{
			if (spase)
			{
				continue;
			}
			else
			{
				spase = true;
				++len;
			}
		}
		else if (c == EOF)
		{
			++len;
			return len;
		}
		else
		{
			++len;
			spase = false;
		}
	}

	return len;
}

void Get_All(FILE* in, char* line, int argc, char** argv)
{
	char* original = line;
	bool include = false;
	char buff[BUFSIZ];
	int iBuff = 0;

	if (!in)
	{
		printf("I can`t open in.txt\n");
		exit(-1);
	}

	char c;
	bool comment = false;

	while (!feof(in))
	{
		c = fgetc(in);
		bool spase = false;

		if (comment)
		{
			if (c == '\n')
				comment = false;
			continue;
		}

		if (c == '?')
		{
			comment = true;
			continue;
		}

		if (isspace(c))
		{
			if (spase)
			{
				continue;
			}
			else
			{
				if (!include)
				{
					spase = true;
					*line++ = ' ';
				}
			}
		}
		else if (c == EOF)
		{
			*line = '\0';
			return;
		}
		else
		{
			if (include)
			{
				if (c == '>')
				{
					if (strlen(buff) > 0)
					{
						int i;
						for (i = 1; i < argc; ++i)
						{
							if (!strcmp(argv[i], buff))
								break;
						}

						if (i != argc)
						{
							FILE* f;
							f = fopen(argv[i], "r");
							if (f)
							{
								Get_All(f, line, argc, argv);
							}
							line = original + strlen(original);
						}
					}
					include = false;
				}
				else
				{
					buff[iBuff] = c;
					++iBuff;
					buff[iBuff] = '\0';
				}
			}
			else
			{
				*line++ = c;
				*line = '\0';
				spase = false;

				if (c == '<')
				{
					int size = strlen(original);
					if (size > 8)
					{
						char* pos = line - 9;
						if (!strcmp(pos, "#include<"))
						{
							include = true;
							line -= 9;
							*line = '\0';
						}
					}
				}
			}


		}
	}
}

std::vector<char*> GetWords(char* buff)
{
	std::vector<char*> words;

	if (!buff)
		return words;

	int i = 0;
	int maxI = strlen(buff);
	char locBuff[BUFSIZ];
	char* ptr = locBuff;
	*ptr = '\0';

	for (; i <= maxI; ++i)
	{
		if (buff[i] == '\0' || buff[i] == '\n' || buff[i] == ' ')
		{
			*ptr = '\0';
			if (strlen(locBuff) > 0)
			{
				char* word = nullptr;
				word = new char[strlen(locBuff) + 1];
				strcpy(word, locBuff);
				words.push_back(word);
				ptr = locBuff;
				*ptr = '\0';
			}
		}
		else if (isSpecialSymbol(buff[i]))
		{
			*ptr = '\0';
			if (strlen(locBuff) > 0)
			{
				char* word = nullptr;
				word = new char[strlen(locBuff) + 1];
				strcpy(word, locBuff);
				words.push_back(word);
				ptr = locBuff;
				*ptr = '\0';
			}
			{
				char* word = nullptr;
				word = new char[2];
				word[0] = buff[i];
				word[1] = '\0';
				words.push_back(word);
			}
		}
		else
		{
			*ptr++ = buff[i];
		}
	}

	return words;
}

bool FindFunctions(std::vector<char*>& content, FunctionStorage& functions)
{
	bool inFunction = false;

	int i = 0;
	int maxI = content.size();

	if (maxI < 3)
		return false;

	for (; i < maxI; ++i)
	{
		if (i+3 < maxI && isType(content[i]) && !isSpecialSymbol(content[i + 1]) && isOpenRound(content[i + 2]))
			//int main(
		{
			Function* findFun = functions.returnFunctionByName(content[i + 1]);

			if (!findFun)//нова€ функци€
			{
				Function curentFunc(content[i + 1]);

				i += 3;

				//аргументы
				while (!isCloseRound(content[i]))
				{
					if (!isComma(content[i]))
					{
						curentFunc.addArg(content[i]);
					}
					++i;
				}
				++i;

				if (isEndOfCommand(content[i]))
				{
					//добавление функции в общий список
					functions.addFunction(curentFunc);
					continue;
				}
				else if (!isOpenFigure(content[i]))
				{
					return false;
				}
				++i;

				//внутренность функции
				while (!isCloseFigure(content[i]))
				{
					curentFunc.addDoing(content[i]);
					++i;
				}

				//добавление функции в общий список
				functions.addFunction(curentFunc);
			}
			else//даем тело сущесвующей функции, если у нее его нет
			{
				if (findFun->GetHaveBody())
					return false;

				Function curentFunc(content[i + 1]);

				i += 3;
				int iArg = 0;

				//аргументы
				while (!isCloseRound(content[i]))
				{
					if (!isComma(content[i]))
					{
						if (strcmp(content[i], findFun->GetArgs()[iArg]))
							return false;
						++iArg;
					}
					++i;
				}
				++i;

				if (!isOpenFigure(content[i]))
				{
					return false;
				}
				++i;

				//внутренность функции
				while (!isCloseFigure(content[i]))
				{
					findFun->addDoing(content[i]);
					++i;
				}
			}
		}
		else
		{
			return false;
		}
	}

}




int main(int argc, char** argv)
{
	if (argc == 1)
		return 1;

	srand(time(0));

	FILE* in;

	in = fopen(argv[1], "r");
	if (!in)
	{
		printf("I can`t open %s\n", argv[1]);
		exit(-1);
	}
	 
	//получение длинны файла
	int sizeLine = 0;
	for (int i = 1; i < argc; ++i)
	{
		sizeLine += SizeOf(argv[i]);
	}

	//объ€вление переменных
	char* line = new char[sizeLine + 1];
	*line = 0;
	FunctionStorage functions;
	ConstuctorOfFuncTree treeConstuctor;
	
	//чтение файла и разбиение на слова
	Get_All(in, line, argc, argv);
	std::vector<char*> content;
	content = GetWords(line);

	//составление дерева

	//поиск всех функций
	if (!FindFunctions(content, functions))
	{
		printf("ERROR in FindFunctions\n");
		exit(1);
	}

	//печать всех функций
	functions.PrintAllFunctions();

	char m[] = "main";
	treeConstuctor.Begin(m);
	int err = functions.Do(m, 0, treeConstuctor);

	switch (err)
	{
	case 0:
		printf("Finished with code 0.\n");
		break;
	case 1:
		printf("ERROR! Function main() not founded!\n");
		break;
	case 2:
		printf("ERROR! Type error!\n");
		break;
	case 3:
		printf("ERROR! Var`s name error!\n");
		break;
	case 4:
		printf("ERROR! I can`t read args!\n");
		break;
	case 6:
		printf("ERROR! I can`t find '(' after name of function!\n");
		break;
	case 8:
		printf("ERROR! Ivalid symbol in func`s args!\n");
		break;
	case 9:
		printf("ERROR! Invalid count of args in func!\n");
		break;
	case 10:
		printf("ERROR! You tru to use fun without body!\n");
		break;
	case 11:
		printf("ERROR! I can`t find ';'!\n");
		break;
	case 99:
		printf("ERROR! Unexpected error!\n");
		break;
	}

	//конец
	fclose(in);
	delete[] line;

	if (!err)
	{
		printf("==================\n");
		treeConstuctor.PrintTreeInConsole();
		printf("==================\n");

		printf("==================\n");
		treeConstuctor.PrintReqInConsole();
		printf("==================\n");
	}

	//system("pause");
	return 0;
}