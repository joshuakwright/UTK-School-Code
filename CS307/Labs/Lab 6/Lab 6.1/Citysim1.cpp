
//Joshua Wright CS307 Gregor Lab 6.1 3/21/21
//This lab takes in a list of cities, populations, coordinates, and various zone data and then outputs them 
//in a formatted list and when the command line argument -graphinfo is put in, it will also output a list
//of distances between each city and a list of time to travel between each city based on what kind of
//city they are.
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <math.h>
#include <sstream>
#include <cmath>

using namespace std;

const float RADIUS = 3982.0;
const float GSPEED = 60.0;
const float ASPEED = 570.0;


class city {
	public:
		string get_name() {return name;}
		string get_type() {return type;}
		int get_zone() {return zone;}
		float get_latitude() {return latitude;}
		float get_longitude() {return longitude;}
		int get_population() {return population;}

		friend istream & operator>>(istream &in, city &c);
		friend ostream & operator<<(ostream &, city &);

	private:
		string name;
		string type;
		int zone;
		float latitude;
		float longitude;
		int population;
};

//input operator for formatted files of cities
istream & operator>>(istream &in, city &c) {
	string temp;
	in >> temp;
	if (temp.at(0) != '#') {
		istringstream sin(temp);
		sin >> c.zone;
		sin.clear();

		in >> c.name >> c.type >> c.latitude >> c.longitude >> c.population;

		c.latitude *= (M_PI / 180.0);
		c.longitude *= (M_PI / 180.0);
		return in;
	}
	else {
		c.zone = -1;
		return in;
	}
}

//output operator of each city's info
ostream & operator<<(ostream &out, city &c) {
	float tlat = c.latitude * (180 / M_PI);
	float tlong = c.longitude * (180 / M_PI);

	out << left << setw(18) << c.name; 
	out << left << setw(12) << c.type;
	out << c.zone << " ";
	out << right << setw(10) << c.population;
	out << right << setw(8) << fixed << setprecision(2) << tlat;
	out << right << setw(8) << fixed << setprecision(2) << tlong;
	out << endl;

	return out;
}

class costtable {
	public:
		costtable();
		float operator()(int, int, int);
		void create_tables(vector<city> &);

	private:
		void create_distancetable(vector<city> &);
		void create_timetable(vector<city> &);

		vector<vector<float> > table;
};

//allows for access of timetable and distance table
float costtable::operator()(int mode, int i, int j) {
	if(i < j) {
		int temp = i;
		i = j;
		j = temp;
	}
	return table[mode][i*(i+1)/2+j];
}

costtable::costtable() {
	table.resize(2);
}

//allows for single function call from main 
void costtable::create_tables(vector<city> &cities) {
	create_distancetable(cities);
	create_timetable(cities);
}

//creates the distance table
void costtable::create_distancetable(vector<city> &cities) {
	int N = (int)cities.size();
	table[0].resize(N*(N+1)/2);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < i; j++) {
			float lat1 = cities[i].get_latitude();
			float lat2 = cities[j].get_latitude();
			float long1 = cities[i].get_longitude();
			float long2 = cities[j].get_longitude();

			float distance;

			distance = 2.0*RADIUS*asin(sqrt(pow(sin((lat2-lat1)/2.0),2)+cos(lat1)*cos(lat2)*pow(sin((long2-long1)/2.0),2)));
			distance = 10.0*round(distance/10.0);

			table[0][i*(i+1)/2+j] = distance;

		}
	}
}

//creates the time table
void costtable::create_timetable(vector<city> &cities) {
	int N = (int)cities.size();
	table[1].resize(N*(N+1)/2);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < i; j++) {
			float distance = table[0][i*(i+1)/2+j];
			string type1 = cities[i].get_type();
			string type2 = cities[j].get_type();

			if (type1 == type2 && type1 == "REGIONAL") {
				table[1][i*(i+1)/2+j] = distance / GSPEED;
			}

			else if ((type1 == type2 && type1 == "GATEWAY") || type1 != type2) {
				table[1][i*(i+1)/2+j] = distance / ASPEED;
			}
		}
	}
}

//function to read in each city info from a file and store for use
void read_cityinfo(vector<city> &cities) { 
	ifstream fin;
	fin.open("city_list.txt");

	city tcity;
	string line;

	while (getline(fin, line)) {
		if (line.length() == 0) {
			getline(fin, line); 
		}
		istringstream sin(line);
		sin >> tcity;
		if (tcity.get_zone() != -1) {
			cities.push_back(tcity);
		}
		sin.clear();
	}	

	fin.close();
}

//writes basic formatted city info to a file
void write_cityinfo(vector<city> &cities) { 
	ofstream fout;
	fout.open("city_info.txt");

	fout << "CITY INFO (N=" << cities.size() << "):\n" << endl;

	for (int i = 0; i < (int)cities.size(); i++) { 
		fout << right << setw(3) << i << " ";
		fout << cities[i];
	}

	fout.close();
}

//writes formatted distance information for each city combo
void write_distancetable(costtable &costs, vector<city> &cities) { 
	ofstream fout;
	fout.open("city_distancetable.txt");

	fout << "DISTANCE TABLE:\n";
	int N = (int)cities.size();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < i; j++) {
			ios oldState(NULL);
			oldState.copyfmt(fout);
			fout << right << setw(3) << i;
			fout << " " << cities[i].get_name() << " to " << cities[j].get_name();
			int n = (int)cities[i].get_name().size() + (int)cities[j].get_name().size() + 4;
			fout << left << setfill('.') << setw(38-n) << " ";
			fout.copyfmt(oldState);
			float temp = costs(0,i,j);
			fout << right << setw(7) << fixed << setprecision(1) << temp; 
			fout << " miles";
			fout << endl;
			fout.copyfmt(oldState);
		}
		fout << endl;
	}
}

//writes formatted travel time between each city to a file
void write_timetable(costtable &costs, vector<city> &cities) { 
	ofstream fout;
	fout.open("city_timetable.txt");

	fout << "TIME TABLE:\n";
	int N = (int)cities.size();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < i; j++) {
			ios oldState(NULL);
			oldState.copyfmt(fout);
			fout << right << setw(3) << i;
			fout << " " << cities[i].get_name() << " to " << cities[j].get_name();
			int n = (int)cities[i].get_name().size() + (int)cities[j].get_name().size() + 4;
			fout << left << setfill('.') << setw(38-n) << " ";
			fout.copyfmt(oldState);
			float temp = costs(1,i,j);
			fout << right << setw(5) << setprecision(1) << fixed << temp;
			fout << " hours";
			fout << endl;
		}
		fout << endl;
	}
}

int main(int argc, char *argv[]) {

	string mode;
	if (argc == 2) {
		mode = argv[1];	
	}

	vector<city> cities;
	read_cityinfo(cities);
	write_cityinfo(cities);
	costtable costs;
	costs.create_tables(cities);

	if (mode == "-graphinfo") {
		write_distancetable(costs, cities);
		write_timetable(costs, cities);

		return 0;
	}
	return 0;
}
