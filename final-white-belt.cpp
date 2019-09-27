#include <iostream>
#include <string>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>

using namespace std;

class Date {
public:
	int getYear() const {
		return year;
	}
	int getMonth() const {
		return month;
	}
	int getDay() const {
		return day;
	}
	void setYear(const int& y) {
		year = y;
	}
	void setMonth(const int& m) {
		month = m;
	}
	void setDay(const int& d) {
		day = d;
	}
private:
	int year;
	int month;
	int day;
};

bool operator<(const Date& lhs, const Date& rhs) {
	if (lhs.getYear() == rhs.getYear() && lhs.getMonth() == rhs.getMonth()) {
		return lhs.getDay() < rhs.getDay();
	} else if (lhs.getYear() == rhs.getYear()) {
		return lhs.getMonth() < rhs.getMonth();
	};
	return lhs.getYear() < rhs.getYear();
}

void printDate(const Date& date) {
	cout << date.getYear() << "-" << setw(2) << setfill('0') << date.getMonth()
		<< "-" << setw(2) << setfill('0') << date.getDay();
}

class Database {
public:
	void AddEvent(const Date& date, const string& event) {
		set<string> events;
		if (!records[date].empty()) events = records[date];
		events.insert(event);
		records.erase(date);
		records.insert (pair <Date, set<string> > (date, events));
	}

	bool DeleteEvent(const Date& date, const string& event) {
		for (auto& item : records[date]) {
				if (item == event) {
					records.at(date).erase(item);
					return true;
				}
		}
		return false;
	}

	int DeleteDate(const Date& date) {
		int del_count = 0;
		//if (!records.at(date).empty()) {
			del_count = records[date].size();
			records.erase(date);
		//}
		return del_count;
	}

	set<string> Find(const Date& date) const {
		if (records.find(date) != records.end())
			return records.at(date);
		return set<string> ();
	}

	void Print() const {
		for (const auto& item: records) {
			for (const auto& e : item.second) {
				cout << setw(4) << setfill('0') << item.first.getYear()
						<< "-" << setw(2) << setfill('0') << item.first.getMonth()
						<< "-" << setw(2) << setfill('0') << item.first.getDay();
				cout << " " << e << endl;
			}
		}
	}
private:
	map<Date, set<string> > records;


};


Date ParseDateAndCheckValidation(const string& s) {
	stringstream stream(s);
	Date date;
	int year = 0, month = 0, day = 0;
	bool err = false;
	stream >> year;
	if (stream.peek() != '-') {
		err = true;
	}
	stream.ignore(1);
	stream >> month;
	if (stream.peek() != '-') {
		err = true;
	}
	stream.ignore(1);
	if (isdigit(stream.peek()) || stream.peek() == '+') {
		stream >> day;
	} else {
		err = true;
	}
	if (stream.peek() != EOF) {
		err = true;
	}
	if (err) {
		string error = "Wrong date format: " + s;
		throw runtime_error(error);
	}
	date.setYear(year);
	if (month > 12 || month < 1) {
		string error = "Month value is invalid: " + to_string(month);
		throw runtime_error(error);
	}
	date.setMonth(month);
	if (day < 1 || day > 31) {
		string error = "Day value is invalid: " + to_string(day);
		throw runtime_error(error);
	}
	date.setDay(day);
	return date;
}



int main() {
	Database db;
	string command;
	try {
		while (getline(cin, command)) {
			stringstream stream(command);
			string action_str="", date_str="", event_str="";
			stream >> action_str;
			if (action_str == "")
				continue;
			if (action_str == "Print") {
				db.Print();
				continue;
			}
			if (action_str == "Add" || action_str == "Del" || action_str == "Find") {
				stream.ignore(1);
				stream >> date_str;
				stream.ignore(1);
				stream >> event_str;
				Date date = ParseDateAndCheckValidation (date_str);
				if (action_str == "Add") {
					db.AddEvent(date, event_str);
				} else if (action_str == "Del" && !event_str.empty()) {
					if (!db.Find(date).empty()) {
						db.DeleteEvent(date, event_str)? (cout << "Deleted successfully" << endl) : (cout << "Event not found" << endl);
					} else {
						cout << "Event not found" << endl;
					}
				} else if (action_str == "Del") {
					if (!db.Find(date).empty()) {
						int deleted = db.DeleteDate(date);
						cout << "Deleted " << deleted << " events" << endl;
					} else {
						cout << "Deleted 0 events" << endl;
					}
				} else if (action_str == "Find") {
					set<string> finded = db.Find(date);
					int s = finded.size(), tmp = 0;
					for (auto& item : finded) {
						cout << item;
						tmp++;
							if (s >= 2 && tmp != s) {
								cout << endl;
							}
					}
					cout << endl;
				}

			} else {
				string error = "Unknown command: " + action_str;
				throw runtime_error(error);
			}
		}
	} catch (runtime_error& e) {
		cout << e.what() << endl;
		return 0;
	}
	return 0;
}
