//main.cpp
//This lab reads in a list of speeding tickets and their metrics and then outputs the fines based on certain condition.
//Joshua Wright
//20-Nov-2019
//Professor: Dr.Marz
//Students worked with:
//Dan Ervine
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;
//global constants
const double INTER_MULT = 5.2252;
const double HIGH_MULT = 9.4412;
const double RES_MULT = 17.1525;
const double NONE_MULT = 12.152;
const string MONTHS[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
//date structure
struct Date {
	int month;
	int day;
	int year;
};
//ticket structure
struct Ticket {
	string citationNum;
	Date issue;
	int speed;
	int speedLim;
	char roadType;
};
//database class
class Database {
	vector<Ticket> tickets;
  public:
	void add_ticket(const Ticket &tempTicket);
	vector<Ticket> gen_report(Date startDate, Date endDate) const;
};
//stand alone function prototypes
bool Read(Database &d, istream &in);

bool Write(const vector<Ticket> &tickets, ostream &out);

int Encode(const Date &d);

int main(int argc, char*argv[]) {
	//error checks to make sure enough arguments were passed into the commandline
	if (argc < 5) {
		cout << "Usage: ./a.out <input file> <start: year-month-day> <end: year-month-day> <output or '-'>\n";
		return 0;
	}
	//error checks the file to see if it can be read
	ifstream in;
	in.open(argv[1]);
	if (in.fail()) {
		cout << "The file '" << argv[1] << "' cannot be read.\n";
		return 0;
	}

	Database d;
	//execute read function
	Read(d, in);

	Date startDate;
	Date endDate;
	int month;
	int day;
	//inputs the start and end date from the command line argument and puts them into a date structure
	stringstream sin;
	sin.str(argv[2]);
	sin >> startDate.year >> month >> day;
	startDate.month = month*-1;
	startDate.day = day*-1;
	sin.clear();
	sin.str(argv[3]);
	sin >> endDate.year >> month >> day;
	endDate.month = month*-1;
	endDate.day = day*-1;
	sin.clear();
	//creates a vector within the correct date range
	vector<Ticket> dateRange = d.gen_report(startDate, endDate);
	
	sin.str(argv[4]);
	char forc;
	sin >> forc;
	sin.clear();
	//outputs the results to the command line or to a file based on user specification
	if (forc == '-') {
		ostringstream out;
		Write(dateRange, out);
		string output;
		output = out.str();
		cout << output;
		out.str("");
	}
	else {
		ofstream out;
		out.open(argv[4]);
		Write(dateRange, out);
		out.close();
	}

	in.close();
	return 0;
}
//puts new tickets into the private ticket vector in the database class
void Database::add_ticket(const Ticket &tempTicket) {
	tickets.push_back(tempTicket);		
}
//this member function of database sorts through the file supplied tickets and then creates a new vector with all the tickets within the specified date range
vector<Ticket> Database::gen_report(Date startDate, Date endDate) const {
	vector<Ticket> dateRange;
	int start = Encode(startDate);
	int end = Encode(endDate);
	//for loop that goes through all the tickets and adds the ones within the specified time range to the dateRange vector
	for (unsigned int i = 0; i < tickets.size() - 1; i++) {
		Ticket tempTicket = tickets.at(i);
		Date tempDate = tempTicket.issue;
		int dateNum = Encode(tempDate);
		if (dateNum <= end && dateNum >= start) {
			dateRange.push_back(tickets.at(i));
		}
	}
	return dateRange;
}
//this fucntion reads in the file line by line and then separates all of the data out into it's respective place in a ticket structure, which is then added to the private tickets vector in the database class
bool Read(Database &d, istream &in) {
	string line;
	Ticket tempTicket;
	bool b = true;
	//while loop that reads in lines until all lines have been read
	while(getline(in, line)) {
		istringstream sin;
		sin.str(line);
		sin >> tempTicket.citationNum >> tempTicket.issue.month >> tempTicket.issue.day >> tempTicket.issue.year >> tempTicket.speed >> tempTicket.speedLim >> tempTicket.roadType;
		d.add_ticket(tempTicket);
		sin.clear();
	}
	//error check that outputs false if the getline doenst work 
	if (in.fail()) {
		b = false;
	}
	return b;
}
//function that calculates the find and then formats all the information in an ostream to either by outputted to the command line or to a file
bool Write(const vector<Ticket> &tickets, ostream &out) {
	//for loop finds fines one at a time and formats it with the rest of the required info
	for (unsigned int i = 0; i < tickets.size(); i++) {
		double fine;
		char road = tickets.at(i).roadType;
		//calculates fine
		switch (road) {
			case 'i':
				fine = (tickets.at(i).speed - tickets.at(i).speedLim) * INTER_MULT;
				break;
			case 'I':
				fine = (tickets.at(i).speed - tickets.at(i).speedLim) * INTER_MULT;
				break;
			case 'R':
				fine = (tickets.at(i).speed - tickets.at(i).speedLim) * RES_MULT;
				break;
			case 'r':
				fine = (tickets.at(i).speed - tickets.at(i).speedLim) * RES_MULT;
				break;
			case 'H':
				fine = (tickets.at(i).speed - tickets.at(i).speedLim) * HIGH_MULT;
				break;
			case 'h':
				fine = (tickets.at(i).speed - tickets.at(i).speedLim) * HIGH_MULT;
				break;
			default:
				fine = (tickets.at(i).speed - tickets.at(i).speedLim) * NONE_MULT;
				break;
		}
		//assigns 3 character month abbreviation to each month value
		string month;
		for (int j = 1; j < 13; j++) {
			if (tickets.at(i).issue.month == j) {
				month = MONTHS[j - 1];
			}
		}
		//accounts for years not in 2xxx format
		int Year = tickets.at(i).issue.year;
		if (tickets.at(i).issue.year < 100) {
			Year += 2000;
		}
		//formatting and string assembly for the final output
		out << right << setw(2) << setfill('0') << tickets.at(i).issue.day;
		out << setfill(' ') << "-" << month << "-" << Year << " " << left << setw(10) << tickets.at(i).citationNum;
		out << " $" << right << setw(9) << fixed << setprecision(2) << fine << '\n';
	}
	//error check outputs false if the ostream was unable to work
	bool a = true;
	if (out.fail()) {
		a = false;
	}
	return a;
}
//encode function that translates date in data structure format to a single integer
int Encode(const Date &d) {
	//accounts for years not in the 2xxx format
	int	tempYear = d.year;
	if (d.year < 100) {
		tempYear += 2000;
	}
	//accounts for months in the 2xxx-xx-xx format
	int tempMonth = d.month;
	if (d.month < 0) {
		tempMonth *= -1;
	}
	//accounts for days in the 2xxx-xx-xx format
	int tempDay = d.day;
	if (d.day < 0) {
		tempDay *= -1;
	}
	//calcualtes single date integer
	int yearVal = tempYear * 10000;
	int monthVal = tempMonth * 100;
	int date = yearVal + monthVal + tempDay;
	return date;
}

