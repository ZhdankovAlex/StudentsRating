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

#include "GroupResult.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

class Course : public vector<GroupResult> 
{
public:
	string name;

	bool Read(const string& name, const string& pathToBaseFolder);
	bool Write(const string& pathToBaseFolder);

	GroupResult& FindOrInsert(const string groupName);
	bool RemoveResult(const string& groupName, int studentIdentity);

	static void List(vector<string>* courses, const string& pathToBaseFolder);
};