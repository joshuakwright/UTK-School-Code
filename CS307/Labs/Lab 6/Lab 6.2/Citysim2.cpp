
//Joshua Wright CS307 Gregor Lab 6.2 3/28/21
//This lab will do all as in lab 6.1 plus it will generate a graph based off of the cities and create adjacency tables based off their zones and types.
//It will then us dijkstras algorithm to sort the graph based on path lenght and output the shortest path between the two given cities, either by time or length. It also has functionality to 
//take int city prefixes or abbreviations and chooses the cities closes to that prefix or if there is a tie, by alphabetical order.
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <math.h>
#include <sstream>
#include <cmath>
#include <set>
#include <map>
#include <limits>
#include <algorithm>

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

void costtable::create_tables(vector<city> &cities) {
	create_distancetable(cities);
	create_timetable(cities);
}

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


void read_cityinfo(vector<city> &cities, map<string,int> &names) { 
	ifstream fin;
	fin.open("city_list.txt");

	city tcity;
	string line;

	int i = 0;
	while (getline(fin, line)) {
		if (line.length() == 0) {
			getline(fin, line); 
		}
		istringstream sin(line);
		sin >> tcity;
		if (tcity.get_zone() != -1) {
			names.insert(pair<string,int>(tcity.get_name(),i));
			i++;
			cities.push_back(tcity);
		}
		sin.clear();
	}	

	fin.close();
}

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
			fout << right << setw(5) << fixed << setprecision(1) << temp;
			fout << " hours";
			fout << endl;
		}
		fout << endl;
	}
}

//fuction to create graph based on zones and regions
void create_citygraph(vector<city> &cities, vector<set<int> > &cgraph, costtable &costs) {
	int N = cities.size();
	cgraph.resize(N);
	vector<int> Reg;
	vector<int> Gat;
	
	//creates two vectors, one of all REGIONAL types and one of all GATEWAY types
	for (int i = 0; i < N; i++) {
		if (cities[i].get_type() == "REGIONAL") {
			Reg.push_back(i);
		}
		else {
			Gat.push_back(i);
		}
	}
	
	for (int i = 0; i < (int)Reg.size(); i++) {
		//creates adjacency if both are regional and in the same zone
		for (int j = 0; j < (int)Reg.size(); j++) {
			if (cities[Reg[i]].get_zone() == cities[Reg[j]].get_zone() && i != j) {
				cgraph[Reg[i]].insert(Reg[j]);
				cgraph[Reg[j]].insert(Reg[i]);
			}
		}
		
		//creates adjacency for regional cities closest to the gateway in the same zone
		int min = 0;
		int m,n;
		int l = 0;
		while (min == 0 && l < (int)Gat.size()) {
			if (cities[Reg[i]].get_zone() == cities[Gat[l]].get_zone()) {
				min = costs(0,Reg[i],Gat[l]);	
				m = i;
				n = l;
			}
			l++;
		}
		for (int k = 0; k < (int)Gat.size(); k++) {
			if (cities[Reg[i]].get_zone() == cities[Gat[k]].get_zone() && costs(0,Reg[i],Gat[k]) < min) {
				min = costs(0,Reg[i],Gat[k]);
				m = i;
				n = k;
			}
		}
		if (min != 0) {
			cgraph[Reg[m]].insert(Gat[n]);
			cgraph[Gat[n]].insert(Reg[m]);
		}
	}

	for (int i = 0; i < (int)Gat.size(); i++) {
		//creates adjacency for all gateway cities in the same zone
		for(int o = 0; o < (int)Gat.size(); o++) {
			if (cities[Gat[i]].get_zone() == cities[Gat[o]].get_zone() && i != o) {
				cgraph[Gat[i]].insert(Gat[o]);
				cgraph[Gat[o]].insert(Gat[i]);
			}
		}

		//creates adjacency for gateway cities nearest to gateway cites in all other zones
		set<int> zones;
		for (int k = 0; k < (int)Gat.size(); k++) {
			zones.insert(cities[Gat[k]].get_zone());
		}
		
		for (int k = 0; k < (int)zones.size(); k++) {
			int min = 0;
			int q,r;
			int s = 0;
			while (min == 0 && s < (int)Gat.size()) {
				if (cities[Gat[i]].get_zone() != cities[Gat[s]].get_zone() && cities[Gat[s]].get_zone() == (k + 1)) {
					min = costs(0,Gat[i],Gat[s]);
					q = i;
					r = s;
				}
				s++;
			}
			for (int t = 0; t < (int)Gat.size(); t++) {
				if (cities[Gat[i]].get_zone() != cities[Gat[t]].get_zone() && costs(0,Gat[i],Gat[t]) < min && cities[Gat[t]].get_zone() == (k + 1)) {
					min = costs(0,Gat[i],Gat[t]);
					q = i;
					r = t;
				}
			}
			if (min != 0 && min <= 5200) {
				cgraph[Gat[q]].insert(Gat[r]);
				cgraph[Gat[r]].insert(Gat[q]);
			}
		}
	}
}

//writes out the contents of the graph
void write_citygraph(vector<set<int> > &cgraph, vector<city> &cities, costtable &costs) {
	ofstream fout;
	fout.open("city_graph.txt");

	fout << "CITY GRAPH:\n\n";

	for (int i = 0; i < (int)cgraph.size(); i++) {
		fout << right << setw(3) << i;
		fout << " " << cities[i].get_name() << endl;
		for (set<int>::iterator it = cgraph[i].begin(); it != cgraph[i].end(); it++) {
			fout << right << setw(6) << *it;
			fout << " " << cities[*it].get_name();
			int n = cities[*it].get_name().size();
			float temp = costs(0,i,*it);
			fout << right << setw(26-n) << fixed << setprecision(1) << temp;
			temp = costs(1,i,*it);
			fout << " miles" << right << setw(6) << fixed << setprecision(1) << temp << " hours\n";
		}
		if (i + 1 != (int)cgraph.size()) {
			fout << endl;
		}
	}

	fout.close();
}

typedef enum {WHITE, BLACK} vcolor_t;

//creates the shortest path using the dijkstra algorithma and then prints out the shortest path
void dijkstra_route(vector<city> &cities, vector<set<int> > &cgraph, costtable &costs, int costmode, int from, int to) { 
	//actual implementation of dijkstra's algorithm
	vector<vcolor_t> vcolor;
	vcolor.assign(cities.size(), WHITE);

	vector<float> vdist;
	vector<int> vlink;

	vdist.assign(cities.size(), numeric_limits<float>::max());
	vdist[from] = 0;

	vlink.assign(cities.size(), -1);
	vlink[from] = from;

	while (1) {
		int next_i = -1;
		float mindist = numeric_limits<float>::max();

		for (int i = 0; i < (int)vcolor.size(); i++) {
			if (vcolor[i] == WHITE && mindist > vdist[i]) {
				next_i = i;
				mindist = vdist[i];
			}
		}

		int i = next_i;
		if (i == -1) {
			return;
		}

		vcolor[i] = BLACK;

		if (i == to) {
			break;
		}

		set<int>::iterator kit = cgraph[i].begin();
		for (int k = 0; k < (int)cgraph[i].size(); k++, kit++) {
			int j = *kit;
			float wij = costs(costmode, i, j);
			if (vcolor[j] == WHITE) {
				if (vdist[j] > vdist[i] + wij) {
					vdist[j] = vdist[i] + wij;
					vlink[j] = i;
				}
			}
		}
	}

	//vlink is sorted and duplicates removed 
	sort(vlink.begin(), vlink.end());
	vector<int>::iterator it = unique(vlink.begin(), vlink.end());
	vlink.resize(distance(vlink.begin(),it));

	//finds the correct cities in vlink based off with cities have the correct travel distance/time between them. 
	int place = to;
	vector<int> path;
	while (place != from) {
		float mincomp = numeric_limits<float>::max();
		bool found = false;
		int tempPlace, tempi;
		for (int i = 0; i < (int)vlink.size(); i++) {
			float comp1 = floor((vdist[place] - vdist[vlink[i]]) * 10 + 0.5) / 10;
			float comp2 = floor(costs(costmode, place, vlink[i]) * 10 + 0.5) / 10;

			if (place != vlink[i] && comp1 == comp2 && comp1 < mincomp) {	
				tempPlace = place;
				tempi = i;
				mincomp = comp1;
				found = true;
			}
		}
		if (found) {
			path.push_back(tempPlace);
			place = vlink[tempi];
		}
	}

	path.push_back(from);

	//this will output the optimal path based off distance
	if (costmode == 0) {
		vector<float> times;
		float time = 0.0;
		times.push_back(time);
		for (int i = (int)path.size() - 1; i > 0; i--) {
			time += costs(1, path[i], path[i - 1]);
			times.push_back(time);
		}

		for (int i = (int)path.size() - 1; i >= 0; i--) {
			cout << right << setw(7) << fixed << setprecision(1) << vdist[path[i]];
			cout << " miles" << right << setw(6) << times[(int)times.size() - i - 1];
			cout << " hours :" << right << setw(3) << path[i];
			cout << " " << left << setw(19) << cities[path[i]].get_name();
			if (i != (int)path.size() - 1) {
				cout << right << setw(6) << costs(0,path[i+1],path[i]);
				cout << " miles" << right << setw(6) << costs(1,path[i+1],path[i]);
				cout << " hours";
			}
			cout << endl;
		}
		cout << endl;
	}
	//this will output the optimal path based off time
	else {
		vector<float> distances;
		float distance = 0.0;
		distances.push_back(distance);
		for (int i = (int)path.size() - 1; i > 0; i--) {
			distance += costs(0, path[i], path[i - 1]);
			distances.push_back(distance);
		}

		for (int i = (int)path.size() - 1; i >= 0; i--) {
			cout << right << setw(7) << fixed << setprecision(1) << distances[(int)distances.size() - i - 1];
			cout << " miles" << right << setw(6) << vdist[path[i]];
			cout << " hours :" << right << setw(3) << path[i];
			cout << " " << left << setw(19) << cities[path[i]].get_name();
			if (i != (int)path.size() - 1) {
				cout << right << setw(6) << costs(0,path[i+1],path[i]);
				cout << " miles" << right << setw(6) << costs(1,path[i+1],path[i]);
				cout << " hours";
			}
			cout << endl;
		}
		cout << endl;
	}
}

//function to find the correct name when an abbreviation or wildcard is given
void nameFinder(string &sfrom, string &sto, int seed, vector<city> &cities) {
	//creates vector of city names for later use
	vector<string> cnames;
	for (int i = 0; i < (int)cities.size(); i++) {
		cnames.push_back(cities[i].get_name());
	}

	//source wild card
	if (sfrom == "*") {
		if (seed != 302 && seed != 307) {
			seed = time(NULL);
		}
		srand(seed);
		int randnum = rand() % ((int)cities.size() - 1);
		sfrom = cities[randnum].get_name();
	}
	//source abbreviation finder
	else {
		vector<int> lengths;
		lengths.assign(cities.size(), 0);
		for (int i = 0; i < (int)cities.size(); i++) {
			int fsize;
			if (cities[i].get_name().length() < sfrom.length()) {
				fsize = cities[i].get_name().length(); 
			}
			else {fsize = sfrom.length();}
			for (int j = 0; j < fsize; j++) {
				if (sfrom.at(j) == cities[i].get_name().at(j)) {
					lengths[i]++;
				}
				else {j = fsize;}
			}
		}

		vector<int> finalind;
		int max = lengths[0];
		for (int i = 0; i < (int)lengths.size(); i++) {
			if (lengths[i] > max) {
				max = lengths[i];
				finalind.resize(1);
				finalind[0] = i;
			}
			else if(lengths[i] == max) {
				finalind.push_back(i);
			}
		}

		if (finalind.size() == 1) {
			sfrom = cities[finalind[0]].get_name();
		}
		else {
			string fcities = cities[finalind[0]].get_name();
			for (int i = 0; i < (int)finalind.size(); i++) {
				if (cities[finalind[i]].get_name() < fcities) {
					fcities = cities[finalind[i]].get_name();
				}
			}
			sfrom = fcities;
		}
	}

	//sink wild card
	if (sto == "*") {
		if (seed != 302 && seed != 307) {
			seed = time(NULL);
		}
		srand(seed);
		int randnum = rand() % ((int)cities.size() - 1);
		while (cities[randnum].get_name() == sfrom) {
			randnum = rand() % ((int)cities.size() - 1);
		}
		sto = cities[randnum].get_name();
	}
	//sink abbreviatio finder
	else {
		vector<int> lengths;
		lengths.assign(cities.size(), 0);
		for (int i = 0; i < (int)cities.size(); i++) {
			int fsize;
			if (cities[i].get_name().length() < sto.length()) {
				fsize = cities[i].get_name().length(); 
			}
			else {fsize = sto.length();}
			for (int j = 0; j < fsize; j++) {
				if (sto.at(j) == cities[i].get_name().at(j)) {
					lengths[i]++;
				}
				else {j = fsize;}
			}
		}

		vector<int> finalind;
		int max = lengths[0];
		for (int i = 0; i < (int)lengths.size(); i++) {
			if (lengths[i] > max) {
				max = lengths[i];
				finalind.resize(1);
				finalind[0] = i;
			}
			else if(lengths[i] == max) {
				finalind.push_back(i);
			}
		}

		if (finalind.size() == 1) {
			sto = cities[finalind[0]].get_name();
		}
		else {
			string fcities = cities[finalind[0]].get_name();
			for (int i = 0; i < (int)finalind.size(); i++) {
				if (cities[finalind[i]].get_name() < fcities) {
					fcities = cities[finalind[i]].get_name();
				}
			}
			sto = fcities;
		}
	}
}

int main(int argc, char *argv[]) {

	string mode;
	int seed;
	if (argc == 2 || argc == 3) {
		mode = argv[1];	
		seed = time(NULL);
	}
	if (argc == 3) {
		string ssead = argv[2];
		ssead.erase(ssead.begin(), ssead.begin() + 6);
		istringstream sin(ssead);
		sin >> seed;
		sin.clear();
	}

	vector<set<int> > cgraph;

	vector<city> cities;
	map<string,int> names;
	read_cityinfo(cities, names);
	write_cityinfo(cities);
	costtable costs;
	costs.create_tables(cities);

	create_citygraph(cities, cgraph, costs);

	if (mode == "-graphinfo") {
		write_distancetable(costs, cities);
		write_timetable(costs, cities);

		write_citygraph(cgraph, cities, costs);

		return 0;
	}

	else if (mode == "-distance" || mode == "-time") {
		string sfrom;
		string sto;
		int costmode;
		if (mode == "-distance") {
			costmode = 0;
		}
		else {costmode = 1;}
		while (1) {
			cout << "Enter> ";
			cin >> sfrom >> sto;

			nameFinder(sfrom, sto, seed, cities);

			int from = names[sfrom];
			int to = names[sto];
			dijkstra_route(cities,cgraph,costs,costmode,from,to); 
		}
	}

	return 0;
}
