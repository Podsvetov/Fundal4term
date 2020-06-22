#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <locale>
#include<Windows.h>

using namespace std;

std::vector<string> split(string str) {
	string buf;
	std::vector<string> result;
	for (int i = 0; i < str.size(); ++i) {
		if (str[i] == ' ' || str[i] == '\t' || str[i] == 0 || i == str.size()) {
			result.push_back(buf);
			buf.clear();
		}
		else {
			buf += str[i];
		}
	}

	result.push_back(buf);

	return result;
}


int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);


	if (argc < 2) {
		cout << "Передайте имена файлов через аргументы";
		return 0;
	}

	map<string, vector<string>> words;

	cout << "Режимы: \n"
		"1. Автоматический\n"
		"2. Обучающий\n" << endl;
	int c;
	bool mode;
	cin >> c;
	switch (c) {
	case 1:
		mode = false;
		break;

	case 2:
		mode = true;
		break;

	default:
		cout << "Ошибка выбора режима" << endl;
	}



	ifstream istream("words.txt");
	string str;
	while (!istream.eof()) {
		string line;
		getline(istream, line);
		vector<string> s = split(line);
		string key = s[0];
		vector<string> syn;
		for (int i = 1; i < s.size(); ++i) {
			if (s[i] == "{" || s[i] == "}") continue;
			syn.push_back(s[i]);
		}
		words.insert(pair<string, vector<string>>(key, syn));
	}
	istream.close();

	vector<string> notFind;
	bool isFind = 0;
	bool coma = false;
	bool dot = false;

	for (int i = 1; i < argc; ++i) {
		istream.open(argv[i]);
		ofstream ostream("tmp.txt");
		while (!istream.eof()) {
			getline(istream, str);
			vector<string> line = split(str);
			for (string& s : line) {
				if (s[s.size()] == '.') {
					dot = true;
					
					s.resize(s.size() - 1);
				}
				if (s[s.size()] == ',') {
					coma = true;
					s.resize(s.size() - 1);
				}

				if (words.find(s) == words.end()) { // ключ не найден
					for (auto& item : words) {
						if (isFind) break;
						for (string& word : item.second) {
							if (word == s) {
								ostream << item.first;
								if (coma) ostream << ",";
								if (dot) ostream << ".";
								ostream << " ";
								isFind = true;
								coma = false;
								dot = false;
								break;
							}
						}
					}
					if (!isFind) {// слово не найдено в словаре
						if (!mode) {
							notFind.push_back(s);
						}
						else {
							cout << "Слово " << s << " не найдено в словаре" << endl;
							cout << "1. Добавить к списку синонимов эталонного слова\n"
								"2. Создать новое эталонное слово\n"
								"3. Заменить эталонное слово\n";

							int chose;
							cin >> chose;
							cout << "Введите эталонное слово" << endl;
							string key;
							cin >> key;
							switch (chose) {
							case 1: {

								auto it = words.find(key);
								if (it == words.end()) {
									cout << "Не найдено эталонное слово";
								}
								else {
									it->second.push_back(s);
								}
								break;
							}


							case 2: {
								
								vector<string> v;
								v.push_back("");
								words.insert(pair<string, vector<string>>(key, v));
								break;
							}

							case 3: {
								auto it = words.find(key);
								if (it == words.end()) {
									cout << "Не найдено эталонное слово";
								}
								else {

									pair<string, vector<string>> p(s, it->second);
									words.erase(key);
									words.insert(p);
								}
								break;
							}

							default:
								cout << "Ошибка выбора действия" << endl;
								ostream.close();
								istream.close();

								return 0;
							}
						}
					}
					isFind = false;
				}
				else {
					ostream << s;
					if (coma) ostream << ",";
					if (dot) ostream << ".";
					coma = false;
					dot = false;
					ostream << " ";
				}
			}
			ostream << endl;
		}
		ofstream NF("notFind.txt");
		if (notFind.size() != 0) {
			NF << endl << endl << "NOT FIND:";
		}

		for (string word : notFind) {
			NF << word << " ";
		}
		NF.close();
		ostream.close();
		istream.close();

		istream.open("tmp.txt");
		ostream.open(argv[i]);
		string line;
		while (!istream.eof()) {
			getline(istream, line);
			ostream << line << endl;
		}

		istream.close();
		ostream.close();



	}


	ofstream wordsOut("words.txt");
	for (auto pair : words) {
		wordsOut << pair.first << " { ";
		for (string str : pair.second) {
			wordsOut << str << " ";
		}
		wordsOut << " }" << endl;
	}
	wordsOut.close();



}