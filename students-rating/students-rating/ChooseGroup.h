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

#include "AttendanceStrategy.h"
#include "AverageStrategy.h"
#include "Course.h"
#include "DailyRating.h"
#include "Group.h"
#include "GroupResult.h"
#include "PlusStrategy.h"
#include "RatingStrategy.h"
#include "Student.h"
#include "StudentRating.h"
#include "TestStrategy.h"

#include "StudentResult.h"
#include "SortType.h"

#include "ResultComparator.h"

#include "PathToBaseFolder.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

bool ChooseGroup(string& group)
{
	vector<string> groups;

	Group::List(&groups, pathToBaseFolder);

	if (groups.size() == 0)
	{
		cout << "No any groups were found";
		return false;
	}

	if (groups.size() > 1)
	{
		cout << "--> Choose group (1 - " << groups.size() << ")" << endl;
	}
	else
	{
		cout << "--> Choose group (only one group)" << endl;
	}

	for (size_t i = 0; i < groups.size(); i++)
	{
		cout << i + 1 << ". " << groups[i] << "\n";
	}

	int groupIndex;
	cin >> groupIndex;
	if (groupIndex < 1 || groupIndex > groups.size())
	{
		cout << "Error! Bad group index, check it!";
		return false;
	}

	group = groups[groupIndex - 1];
	return true;
}