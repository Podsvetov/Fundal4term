#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include<string>
#include<Windows.h>

#include "Dictionary.h"

using namespace std;

bool isEnd(char c)
{
    return c == ' ' || c == ',' || c == ':' || c == '.' || c == '\n' || c == '—' || c == '!' || c == '?' || c == '«' || c == '»' ||
        c == '…' || c == ';';
}


char* createFileOutName(char* fileInName)
{
    char* fileOutName = new char[strlen(fileInName) + 5];
    strcpy(fileOutName, fileInName);
    char* ptr = fileOutName + strlen(fileInName) - 4;

    *ptr++ = '-';
    *ptr++ = 'o';
    *ptr++ = 'u';
    *ptr++ = 't';

    *ptr++ = '.';
    *ptr++ = 't';
    *ptr++ = 'x';
    *ptr++ = 't';

    *ptr = '\0';

    return fileOutName;
}






int main(int argc, char** argv)
{
    if (argc < 2)
    {
        return 0;
    }

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

    Dictionary dict;
    if (!dict.GetDict("dictpro.txt"))
    {
        exit(3);
    }

	FILE* in;
    FILE* out;

    for (int fileId = 1; fileId < argc; ++fileId)
    {
        in = fopen(argv[fileId], "r");
        if (!in)
        {
            printf("Error! File %s not founded!\n", argv[fileId]);
            continue;
        }

        char* fileOutName = createFileOutName(argv[fileId]);
        out = fopen(fileOutName, "w");
        if (!out)
        {
            fclose(in);
            exit(2);
        }
        delete[] fileOutName;

        char buff[BUFSIZ];
        char* startWord = nullptr;
        char* endWord = nullptr;

        while (!feof(in))
        {
            //INPUT

            char c = ' ';
            c = getc(in);
            char* ptr = buff;
            bool end = false;

            while (!feof(in))
            {
                if (isEnd(c))
                {
                    *ptr = '\0';
                    int id = dict.FindWord(buff);
                    if (id != -1)
                    {
                        int countToTry = 30;
                        bool done = false;
                        int randSetting = 5;

                        while (!done && countToTry)
                        {
                            int r = id + (rand() % (randSetting * 2)) - randSetting;
                            DictWord* currentWord = dict.GetById(id);
                            DictWord* randWord = dict.GetById(r);
                            if (currentWord && randWord && Equal(currentWord->word, randWord->word))
                            {
                                //fprintf(out, "{%s}(%d)%c", randWord->word, countToTry, c);
                                fprintf(out, "%s%c", randWord->word, c);
                                done = true;
                            }

                            ++randSetting;
                            --countToTry;
                        }

                        if (!done)
                        {
                            //fprintf(out, "{%s}(!)%c", buff, c);
                            fprintf(out, "%s%c", buff, c);
                        }
                    }
                    else
                    {
                        //fprintf(out, "{%s}%c", buff, c);
                        fprintf(out, "%s%c", buff, c);
                    }
                    ptr = buff;
                    c = getc(in);
                }
                else
                {
                    *ptr = c;
                    ++ptr;
                    c = getc(in);
                }
            }

        }

        fclose(in);
        fclose(out);
    }
	
	return 0;
}