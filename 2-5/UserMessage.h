#pragma once
class UserMessage
{
public:
	UserMessage(char*, SYSTEMTIME&, char*);
	~UserMessage();

	char* userName;
	SYSTEMTIME time;
	char* message;

	void Print();
	void PrintName();
	void PrintTime();

	bool operator<(const UserMessage& other);
	bool operator==(const UserMessage& other);
	bool operator>(const UserMessage& other);

private:
	UserMessage();
};

bool operator<(const SYSTEMTIME& first, const SYSTEMTIME& second);

bool operator<=(const SYSTEMTIME& first, const SYSTEMTIME& second);
bool operator>=(const SYSTEMTIME& first, const SYSTEMTIME& second);