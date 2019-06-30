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

#include "json.hpp"

using namespace std;
using nlohmann::json;

class DailyRating 
{
public:
	DailyRating() : attendance(false), plus(false), test(0.0) 
	{

	}

	DailyRating(const json& sourceJson);

	bool attendance;
	bool plus;
	double test;
};