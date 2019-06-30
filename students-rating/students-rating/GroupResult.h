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

#include "StudentRating.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

class GroupResult 
{
public:
	GroupResult() 
	{

	}

	GroupResult(const json& sourceJson);

	StudentRating& FindOrInsert(int studentIdentity);

	string group;
	map<int, StudentRating> progress;
};