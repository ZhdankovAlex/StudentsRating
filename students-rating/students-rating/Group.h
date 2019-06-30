#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include <iomanip>
#include <locale>
#include <algorithm>

#ifdef WIN32
#include <direct.h>
#include <windows.h>
#else
#include <sys/stat.h>
#include <dirent.h>
#endif

#include "Student.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

class Group 
{
public:

	string name;
	vector<Student> students;

	bool Read(const string& name, const string& pathToBaseFolder);
	bool Write(const string& pathToBaseFolder) const;

	int FindStudent(const string& studentName) const;
	int FindStudent(int id) const;

	static void List(vector<string>* groups, const string& pathToBaseFolder);
};