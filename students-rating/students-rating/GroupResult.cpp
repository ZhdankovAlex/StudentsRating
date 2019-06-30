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

#include "Course.h"
#include "GroupResult.h"
#include "StudentRating.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

GroupResult& Course::FindOrInsert(const string groupName) 
{
	for (iterator i = begin(); i != end(); i++)
	{
		if (i->group.compare(groupName) == 0) 
		{
			return *i;
		}
	}

	GroupResult groupResult;
	groupResult.group = groupName;
	push_back(groupResult);
	return back();
}

GroupResult::GroupResult(const json& source) 
{
	group = source["group"].get<string>();

	const json& progressJson = source["progress"];

	for (json::const_iterator it = progressJson.begin(); it != progressJson.end(); ++it)
	{
		int id = atoi(it.key().c_str());
		StudentRating studentRating(it.value());
		progress[id] = studentRating;
	}
}