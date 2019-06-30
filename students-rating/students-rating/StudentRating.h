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

#include "DailyRating.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

class StudentRating : public map<string, DailyRating> 
{
public:
	StudentRating() 
	{

	}

	StudentRating(const json& source);

	DailyRating& FindOrInsert(const string date);
};