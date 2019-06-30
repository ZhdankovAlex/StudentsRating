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
#include "GroupResult.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

StudentRating& GroupResult::FindOrInsert(int studentIdentity) 
{
	map<int, StudentRating>::iterator findLocal = progress.find(studentIdentity);
	if (findLocal != progress.end()) 
	{
		return findLocal->second;
	}

	StudentRating studentRating;
	progress[studentIdentity] = studentRating;
	return progress[studentIdentity];
}

StudentRating::StudentRating(const json& source)
{	
	for (json::const_iterator it = source.begin(); it != source.end(); ++it)
	{
		string date = it.key();
		DailyRating dailyRating(it.value());
		(*this)[date] = dailyRating;
	}
}