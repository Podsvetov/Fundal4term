#include <fstream>
#include "Entry.h"
#include "RBTree.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#pragma warning(disable : 4996)
using namespace std;
using namespace boost::gregorian;

string cutFirst(string s) {
    s.erase(0, 1);
    return s;
}

std::vector<string> split(string str) {

    string buf;
    std::vector<string> result;
    if (str == "") return result;
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == 0 || i == str.size()) {
            if (buf.size() != 0) {
                result.push_back(buf);
                buf.clear();
            }
        }
        else {
            buf += str[i];
        }
    }

    result.push_back(buf);

    return result;
}

void saveInFile(string fileName, RBTree<string, Entry> tree) {
    vector<RBTree<string, Entry>*>* v = new vector<RBTree<string, Entry>*>();
    tree.getAsVector(v, tree.root);
    ofstream ostream(fileName);
    for (auto* t : *v) {
        ostream << *t->getData() << endl;
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    if (argc < 2) {
        cout << "Недостаточно аргументов" << endl;
        return -1;
    }
    RBTree< std::string, Entry> tree;
    Entry* entry;

    ifstream istream(argv[1]);
    bool inEntry = false;
    string line;
    while (!istream.eof()) {
        getline(istream, line);
        vector<string> words = split(line);
        if (!inEntry && words.size() != 0 && words[0] != "{") {
            entry = new Entry(line);
        }
        else if (words.size() != 0 && words[0] == "{") {
            inEntry = true;
        }
        else if (words.size() != 0 && words[0] == "}") {
            tree.add(entry->getFIO(), entry);
            inEntry = false;
        }
        else if (words.size() > 0) {
            int num = stoi(cutFirst(words[1]));
            const std::locale loc = std::locale(std::locale::classic(),
                new boost::posix_time::time_input_facet("%d.%m.%Y"));
            std::istringstream is(words[3]);
            is.imbue(loc);

            boost::posix_time::ptime start;
            is >> start;

           // is = istringstream(words[5]);
            //is.imbue(loc);

            boost::posix_time::ptime end;
            if (words[5] == "nd") {

                time_t seconds = time(NULL);
                tm* timeinfo = localtime(&seconds);
                timeinfo->tm_year += 1900; timeinfo->tm_mon += 1;
                stringstream s1, s2, s3;
                s1 << timeinfo->tm_mday; s2 << timeinfo->tm_mon; s3 << timeinfo->tm_year;
                string vrem = s1.str().append(".").append(s2.str()).append(".").append(s3.str());
                is = istringstream(vrem);
                is.imbue(loc);
            }
            else {
                is = istringstream(words[5]); is.imbue(loc);
            }
            is >> end;
            // cout << end;


            int work = stoi(words[7]);
            int price = stoi(words[9]);
            entry->addRecord(Record(num, start, end, work, price));

        }
        else if (words.size() != 0) {
            cout << "Smthg wrng" << endl;
        }
    }

    istream.close();


    cout << "Введите ФИО:" << endl;
    string fio;
    getline(cin, fio);
    auto node = tree.findNode(fio);
    if (node == nullptr) {
        cout << "Сотрудник с введеными ФИО не найден" << endl;
        return 0;
    }
    int size = node->getData()->getRecords().size();
    int price = 0;
    if (size != 0) {
        auto maxDDate = node->getData()->getRecords()[0].getEnd() - node->getData()->getRecords()[0].getStart();

        int numOfMaxDDate = 0;
        int maxPrice = 0;
        int maxPriceNum = 0;
        for (int j = 0; j < size; ++j) {
            Record record = node->getData()->getRecords()[j];
            price += record.getPrice();
            auto dDate = record.getEnd() - record.getStart();
            if (dDate > maxDDate) {
                maxDDate = dDate;
                numOfMaxDDate = j;
            }
            if (record.getPrice() > maxPrice) {
                maxPrice = record.getPrice();
                maxPriceNum = j;
            }
        }

        cout << "Стоймость всех договоров: " << price << endl;
        cout << "Список всех договоров " << *node->getData();
        cout << "Самый продолжительный договор №" << node->getData()->getRecords()[numOfMaxDDate].getNumber() << endl;
        cout << "Самый дорогой договор № " << node->getData()->getRecords()[maxPriceNum].getNumber() << endl;
    }
    else {
        cout << "Записей нет" << endl;
    }
    while (true) {
        cout << "1. Удалить сотрудника \n"
            "2. Удалить договор\n"
            "3. Записать в файл и выйти\n";

        int n;
        cin >> n;

        switch (n) {
        case 1: {
            tree.deleteNode(node);
            saveInFile(argv[1], tree);
            return 0;
        }

        case 2: {
            int numberOfRecord;
            cout << "Введите номер договора";
            cin >> numberOfRecord;
            vector<Record> records = node->getData()->getRecords();
            for (int j = 0; j < records.size(); ++j) {
                if (records[j].getNumber() == numberOfRecord) {
                    records.erase(records.begin() + j);
                    Entry* e = node->getData();
                    e->setRecords(records);
                    node->setData(e);
                    break;
                }
            }

        }
              break;

        case 3: {
            saveInFile(argv[1], tree);


            return 0;
        }
        }
    }

}