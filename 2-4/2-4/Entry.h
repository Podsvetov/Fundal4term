#pragma once


#ifndef INC_2_ENTRY_H
#define INC_2_ENTRY_H

#include "boost/date_time/gregorian/gregorian.hpp"
#include <string>
#include <vector>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class Record {
private:
    int number;
    boost::posix_time::ptime start;
    boost::posix_time::ptime end;
    int work;
    int price;

public:
    Record(int n, boost::posix_time::ptime s, boost::posix_time::ptime e, int w, int p) {
        number = n;
        start = s;
        end = e;
        work = w;
        price = p;
    }

    Record() = default;

    int getNumber() const {
        return number;
    }

    const boost::posix_time::ptime& getStart() const {
        return start;
    }

    const boost::posix_time::ptime& getEnd() const {
        return end;
    }

    int getWork() const {
        return work;
    }

    int getPrice() const {
        return price;
    }
};

class Entry {

private:

    std::string FIO;
    std::vector<Record> records;

public:
    Entry(std::string str) {
        FIO = str;
    }

    std::string getFIO() {
        return FIO;
    }

    void setFIO(std::string fio) {
        this->FIO = fio;
    }

    void addRecord(Record r) {
        this->records.push_back(r);
    }

    std::vector<Record> getRecords() {
        return records;
    }

    void setRecords(std::vector<Record> r) {
        records = r;
    }

    friend std::ostream& operator<< (std::ostream& out, const Entry& entry);

};

std::ostream& operator<<(std::ostream& out, const Entry& entry) {
    out << entry.FIO << std::endl;
    out << "{" << std::endl;
    std::stringstream stream;
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
    facet->format("%d.%m.%Y");
    stream.imbue(std::locale(std::locale::classic(), facet));


    for (int j = 0; j < entry.records.size(); ++j) {
        Record r = entry.records[j];
        stream << r.getStart();
        out << "Договор N" << r.getNumber() << " нач. " << stream.str() << " кон. ";
        stream.str("");
        stream << r.getEnd() << " ";
        out << stream.str() << " Работа " << r.getWork() << " Стоимость " << r.getPrice() << std::endl;
        stream.str("");
    }

    out << std::endl << "}" << std::endl;
    return out;
}


#endif //INC_2_ENTRY_H
