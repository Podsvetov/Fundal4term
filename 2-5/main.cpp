#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include<Windows.h>
#include <vector>
#include <cstdio>

#include "UserMessage.h"
#include "AVL_Tree.h"


void GetString(FILE* f, char* buff, int maxSize)
{
    if (!f || !buff)
        return;

    *buff = '\0';
    int count = 0;
    char c = ' ';

    c = getc(f);

    while (!feof(f) && c != '\n' && count < (maxSize - 2))
    {
        *buff++ = c;
        ++count;
        c = getc(f);
    }

    *buff = '\0';
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
        switch (buff[i])
        {
        case '\0':
        case ' ':
        case '.':
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
            break;
        case '�':
            break;
        case ';':
            break;
        default:
            *ptr++ = buff[i];
            break;
        }
    }

    return words;
}

bool isDgt(char c)
{
    if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' ||
        c == '8' || c == '9')
        return true;
    return false;
}

bool isNum(char* buff)
{
    if (!buff)
        return false;

    int size = strlen(buff);
    for (int i = 0; i < size; ++i)
    {
        if (!isDgt(buff[i]))
            return false;
    }

    return true;
}

bool CreateSysTime(SYSTEMTIME& time, char* buff)
{
    enum NowGet {d, m, y, h, min, s, ms, ok} now = NowGet::d;
    char* ptr = buff;

    while (*ptr != '\0')
    {
        while (*ptr != ' ' && *ptr != '\0')
        {
            ++ptr;
        }

        *ptr = '\0';

        switch (now)
        {
        case d:
            time.wDay = atoi(buff);
            now = m;
            break;
        case m:
            time.wMonth = atoi(buff);
            now = y;
            break;
        case y:
            time.wYear = atoi(buff);
            now = h;
            break;
        case h:
            time.wHour = atoi(buff);
            now = min;
            break;
        case min:
            time.wMinute = atoi(buff);
            now = s;
            break;
        case s:
            time.wSecond = atoi(buff);
            now = ms;
            break;
        case ms:
            time.wMilliseconds = atoi(buff);
            now = ok;
            break;
        case ok:
            return true;
            break;
        }

        ++ptr;

        buff = ptr;
    }

    if (now == NowGet::ok)
    {
        return true;
    }

    return false;
}

UserMessage* CreateMsg(char* buff)
{
    int count;

    // Get name
    if (*buff != '<')
    {
        return nullptr;
    }
    ++buff;
    char name[BUFSIZ];
    count = 0;
    while (*buff != '>' && count < (BUFSIZ - 1))
    {
        name[count] = *buff;
        ++count;
        ++buff;
    }
    name[count] = '\0';
    if (*buff != '>')
    {
        return nullptr;
    }
    ++buff;

    // Get time
    if (*buff != '<')
    {
        return nullptr;
    }
    ++buff;
    char timeStr[BUFSIZ];
    count = 0;
    while (*buff != '>' && count < (BUFSIZ - 1))
    {
        timeStr[count] = *buff;
        ++count;
        ++buff;
    }
    timeStr[count] = '\0';
    if (*buff != '>')
    {
        return nullptr;
    }
    ++buff;

    // Get txt
    if (*buff != '<')
    {
        return nullptr;
    }
    ++buff;
    char txt[BUFSIZ];
    count = 0;
    while (*buff != '>' && count < (BUFSIZ - 1))
    {
        txt[count] = *buff;
        ++count;
        ++buff;
    }
    txt[count] = '\0';
    if (*buff != '>')
    {
        return nullptr;
    }
    ++buff;

    SYSTEMTIME time;
    if (!CreateSysTime(time, timeStr))
        return nullptr;

    return new UserMessage(name, time, txt);
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    AVL_Tree tree;

    FILE* in;
    char buff[BUFSIZ];
    UserMessage* msg = nullptr;

    //////////// Read
    
    in = fopen("in.txt", "r");
    if (!in)
    {
        exit(1);
    }

    while (!feof(in))
    {
        GetString(in, buff, BUFSIZ);

        if (strlen(buff) > 0)
        {
            msg = CreateMsg(buff);

            if (msg)
            {
                if ((tree.find(msg)) != NULL)
                {
                    printf("Insert error!!! ");
                    msg->Print();
                    printf("\n");
                    delete msg;
                }
                else
                {
                    tree.insert(msg);
                    printf("OK insert!!! ");
                    msg->Print();
                    printf("\n");
                }
            }

            msg = nullptr;
        }
    }

    fclose(in);
    
    ////// Work


    char cmd[BUFSIZ];
    std::vector<UserMessage*> messages;
    AVL_Node* nodeMes = nullptr;
    printf("[�����::][������] - ������ ������ �������\n");

    do
    {
        if (messages.size() > 0)
        {
            messages[0]->PrintName();
            printf("::");
        }
        else
        {
            printf("�����::");
        }


        gets_s(cmd);
        if (!strcmp(cmd, "������"))
        {
            printf("\t[�����::][������] - ������ ������ ������\n");
            printf("\t[�����::][������] - ����� � ������� ������������\n");
            printf("\t[�����::][�� �������] - ����� ��������� �� ���������� ���������\n");

            printf("\t[������������::][���] - ��� ��������� ������������\n");
            printf("\t[������������::][�� �������] - ����� ��������� ������������ �� ���������� ���������\n");
            printf("\t[������������::][�������] - ����� ���������� ������� ��������� ��� ��������\n");
            printf("\t[������������::][������� ���] - ������� ��� ��������� ������������\n");
            printf("\t[������������::][�����] - �������� �� ����� ��������������\n");
        }
        else if (!strcmp(cmd, "������"))
        {
            if(messages.size() > 0)
            {
                printf("������������ ��� ������! ��������� ����� � ������� [�����].\n");
            }
            else
            {
                printf("������� UserName ������������: ");
                gets_s(cmd);
                tree.findMessagesByName(messages, cmd);
                if (messages.size() == 0)
                    printf("����� ������������ �� ������!\n");
            }
        }
        else if (!strcmp(cmd, "���"))
        {
            if(messages.size() > 0)
            {
                int size = messages.size();
                for (int i = 0; i < size; ++i)
                {
                    printf("<id:%d>", i);
                    messages[i]->Print();
                    printf("\n");
                }
            }
            else
                printf("������������ �� ������! �������� ������������ � ������� [������].\n");
        }
        else if (!strcmp(cmd, "�� �������"))
        {
            bool ok = true;
            std::vector<UserMessage*> timeMessages;
            SYSTEMTIME timeStart, timeEnd;

            printf("������� ������ ������ (d m y h min s ms).\n");
            gets_s(cmd);
            if (!CreateSysTime(timeStart, cmd))
            {
                ok = false;
            }

            if (ok)
            {
                printf("������� ����� ������ (d m y h min s ms).\n");
                gets_s(cmd);
                if (!CreateSysTime(timeEnd, cmd))
                {
                    ok = false;
                }
            }

            if (ok)
            {
                if (timeEnd < timeStart)
                    ok = false;
            }

            if (ok)
            {
                if (messages.size() > 0)
                {
                    int size = messages.size();
                    for (int i = 0; i < size; ++i)
                    {
                        if (messages[i]->time >= timeStart && messages[i]->time <= timeEnd)
                        {
                            timeMessages.push_back(messages[i]);
                        }
                    }

                    size = timeMessages.size();
                    for (int i = 0; i < size; ++i)
                    {
                        timeMessages[i]->Print();
                        printf("\n");
                    }
                }
                else
                {
                    tree.findMessagesByTime(timeMessages, timeStart, timeEnd);
                    int size = timeMessages.size();
                    for (int i = 0; i < size; ++i)
                    {
                        timeMessages[i]->Print();
                        printf("\n");
                    }
                }
            }
            else
            {
                printf("������ �����.\n");
            }
        }
        else if (!strcmp(cmd, "�������"))
        {
            if(messages.size() > 0)
            {
                int id = -1;
                while (id == -1)
                {
                    printf("������� id ���������, ������� �� ������ �������. ������ id ���� ��������� �������� ������������ ����� � ������� [���].\n������� [�����], ����� ���������.\n");
                    printf("id: ");
                    gets_s(cmd);

                    if (isNum(cmd))
                    {
                        id = atoi(cmd);
                    }
                    else if (!strcmp(cmd, "���"))
                    {
                        int size = messages.size();
                        for (int i = 0; i < size; ++i)
                        {
                            printf("<id:%d>", i);
                            messages[i]->Print();
                            printf("\n");
                        }
                    }
                    else if(!strcmp(cmd, "�����"))
                    {
                        id = -2;
                    }
                    else
                        printf("����������� ��������\n");
                }

                if (id != -2)
                {
                    if (id < messages.size() && id > -1)
                    {
                        char* name = new char[strlen(messages[0]->userName) + 1];
                        strcpy(name, messages[0]->userName);

                        tree.removeMsg(messages[id]);
                        messages.clear();

                        tree.findMessagesByName(messages, name);
                        delete[] name;

                        printf("��������� �������.\n");
                    }
                    else
                    {
                        printf("��������� � ����� id �� ��������.\n");
                    }
                }
            }
            else
                printf("������������ �� ������! �������� ������������ � ������� [������].\n");
        }
        else if (!strcmp(cmd, "������� ���"))
        {
            if (messages.size() > 0)
            {
                int size = messages.size();
                for (int i = 0; i < size; ++i)
                {
                    tree.removeMsg(messages[i]);
                }
                messages.clear();
                printf("��������� �������.\n");
            }
            else
                printf("������������ �� ������! �������� ������������ � ������� [������].\n");
        }
        else if (!strcmp(cmd, "�����"))
        {
            if(messages.size() > 0)
            {
                messages.clear();
            }
            else
                printf("������������ �� ������! �������� ������������ � ������� [������].\n");
        }
        else if (!strcmp(cmd, "����"))
        {
            printf("����������� ������\n");
        }
        else
        {
            printf("����������� ��������\n");
        }

    } while (strcmp(cmd, "����"));
}