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
#include "StudentRating.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

DailyRating& StudentRating::FindOrInsert(const string date) 
{
	iterator findLocal = find(date);
	if (findLocal != end()) 
	{
		return findLocal->second;
	}

	DailyRating dailyRating;
	(*this)[date] = dailyRating;
	return (*this)[date];
}

DailyRating::DailyRating(const json& source)
{
	plus = false;
	attendance = false;
	test = 0;
	if (source.find("plus") != source.end())
	{
		plus = source["plus"].get<bool>();
	}
	if (source.find("attendance") != source.end())
	{
		attendance = source["attendance"].get<bool>();
	}
	if (source.find("test") != source.end())
	{
		test = source["test"].get<double>();
	}
}