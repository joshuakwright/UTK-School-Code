
#ifndef __PERSON_H__ 
#define __PERSON_H__

#include <vector>
using namespace std;

enum type_enum { UNKNOWN=-1, FACULTY=0,  ASST_PROF=1, ASSOC_PROF=2, FULL_PROF=3,
                 STUDENT=10, FRESHMAN=11, SOPHOMORE=12, JUNIOR=13, SENIOR=14 };

class person {
	public:
		bool operator<(const person &rhs) const;
		friend ostream & operator<<(ostream &, person &);
		string get_name() { return name; }
		type_enum get_type() { return type; }
		type_enum get_category() { return category; }
		string print_type;

	protected:
		string name;
		type_enum type;
		type_enum category;
		
		virtual void print_personinfo(ostream &) = 0;
		virtual void print_courseinfo(ostream &) = 0;

};

class student : public person {
	public:
		student(string, type_enum, type_enum, vector<string>, vector<double>);
		bool operator<(const person &rhs) const;

	private:
		vector<string> courselist;
		vector<double> gp;
		void print_personinfo(ostream &);
		void print_courseinfo(ostream &);
};

class faculty : public person {
	public:
		faculty(string, type_enum, type_enum, vector<string>);
		bool operator<(const person &rhs) const;

	private:
		vector<string> courselist;
		void print_personinfo(ostream &);
		void print_courseinfo(ostream &);
};

#endif

