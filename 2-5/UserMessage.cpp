#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>

#include "UserMessage.h"

UserMessage::UserMessage(char* _userName, SYSTEMTIME& _time, char* _message) : userName(nullptr), message(nullptr)
{
	if (_userName)
	{
		userName = new char[sizeof(_userName) + 1];
		strcpy(userName, _userName);
	}

	if (_message)
	{
		message = new char[sizeof(_message) + 1];
		strcpy(message, _message);
	}

	time = _time;
}

UserMessage::~UserMessage()
{
	delete[] userName;
	delete[] message;
}

void UserMessage::Print()
{
	printf("<%s><%d.%d.%d %d:%d:%d:%d><%s>", userName, 
		time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
		message);
}

void UserMessage::PrintName()
{
	printf("<%s>", userName);
}

void UserMessage::PrintTime()
{
	printf("<%d.%d.%d %d:%d:%d:%d>", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
}



///// OPERATORS

bool UserMessage::operator<(const UserMessage& other)
{
	if (!userName && !other.userName)
		return false;

	if (!userName)
		return true;

	if (!other.userName)
		return false;

	if (strcmp(userName, other.userName) < 0)
	{
		return true;
	}
	else
	{
		unsigned long long timeLong = 0;
		unsigned long long timeLongOther = 0;

		{
			FILETIME f;
			SystemTimeToFileTime(&time, &f);
			ULARGE_INTEGER    lv_Large;
			lv_Large.LowPart = f.dwLowDateTime;
			lv_Large.HighPart = f.dwHighDateTime;

			timeLong = lv_Large.QuadPart;
		}

		{
			FILETIME f;
			SystemTimeToFileTime(&other.time, &f);
			ULARGE_INTEGER    lv_Large;
			lv_Large.LowPart = f.dwLowDateTime;
			lv_Large.HighPart = f.dwHighDateTime;

			timeLongOther = lv_Large.QuadPart;
		}

		return (timeLong < timeLongOther);
	}
}

bool UserMessage::operator==(const UserMessage& other)
{
	if (!userName && !other.userName)
		return true;

	if (!userName)
		return false;

	if (!other.userName)
		return false;

	if (!strcmp(userName, other.userName))
	{
		unsigned long long timeLong = 0;
		unsigned long long timeLongOther = 0;

		{
			FILETIME f;
			SystemTimeToFileTime(&time, &f);
			ULARGE_INTEGER    lv_Large;
			lv_Large.LowPart = f.dwLowDateTime;
			lv_Large.HighPart = f.dwHighDateTime;

			timeLong = lv_Large.QuadPart;
		}

		{
			FILETIME f;
			SystemTimeToFileTime(&other.time, &f);
			ULARGE_INTEGER    lv_Large;
			lv_Large.LowPart = f.dwLowDateTime;
			lv_Large.HighPart = f.dwHighDateTime;

			timeLongOther = lv_Large.QuadPart;
		}

		return (timeLong == timeLongOther);
	}
	else
	{
		return false;
	}
}

bool UserMessage::operator>(const UserMessage& other)
{
	return (!operator<(other) && !operator==(other));
}

/////////////////// SYSTIME

bool operator<(const SYSTEMTIME& first, const SYSTEMTIME& second)
{
	unsigned long long firstLong = 0;
	unsigned long long secondLong = 0;

	{
		FILETIME f;
		SystemTimeToFileTime(&first, &f);
		ULARGE_INTEGER    lv_Large;
		lv_Large.LowPart = f.dwLowDateTime;
		lv_Large.HighPart = f.dwHighDateTime;

		firstLong = lv_Large.QuadPart;
	}

	{
		FILETIME f;
		SystemTimeToFileTime(&second, &f);
		ULARGE_INTEGER    lv_Large;
		lv_Large.LowPart = f.dwLowDateTime;
		lv_Large.HighPart = f.dwHighDateTime;

		secondLong = lv_Large.QuadPart;
	}

	return (firstLong < secondLong);
}

bool operator<=(const SYSTEMTIME& first, const SYSTEMTIME& second)
{
	unsigned long long firstLong = 0;
	unsigned long long secondLong = 0;

	{
		FILETIME f;
		SystemTimeToFileTime(&first, &f);
		ULARGE_INTEGER    lv_Large;
		lv_Large.LowPart = f.dwLowDateTime;
		lv_Large.HighPart = f.dwHighDateTime;

		firstLong = lv_Large.QuadPart;
	}

	{
		FILETIME f;
		SystemTimeToFileTime(&second, &f);
		ULARGE_INTEGER    lv_Large;
		lv_Large.LowPart = f.dwLowDateTime;
		lv_Large.HighPart = f.dwHighDateTime;

		secondLong = lv_Large.QuadPart;
	}

	return (firstLong <= secondLong);
}

bool operator>=(const SYSTEMTIME& first, const SYSTEMTIME& second)
{
	unsigned long long firstLong = 0;
	unsigned long long secondLong = 0;

	{
		FILETIME f;
		SystemTimeToFileTime(&first, &f);
		ULARGE_INTEGER    lv_Large;
		lv_Large.LowPart = f.dwLowDateTime;
		lv_Large.HighPart = f.dwHighDateTime;

		firstLong = lv_Large.QuadPart;
	}

	{
		FILETIME f;
		SystemTimeToFileTime(&second, &f);
		ULARGE_INTEGER    lv_Large;
		lv_Large.LowPart = f.dwLowDateTime;
		lv_Large.HighPart = f.dwHighDateTime;

		secondLong = lv_Large.QuadPart;
	}

	return (firstLong >= secondLong);
}