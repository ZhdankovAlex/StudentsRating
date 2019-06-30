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

bool ChooseCourse(string& course)
{
	vector<string> courses;

	Course::List(&courses, pathToBaseFolder);

	if (courses.size() == 0)
	{
		cout << "No any courses were found";
		return false;
	}

	if (courses.size() > 1)
	{
		cout << "--> Choose course (1 - " << courses.size() << ")" << endl;
	}
	else
	{
		cout << "--> Choose course (only one course)" << endl;
	}

	for (size_t i = 0; i < courses.size(); i++)
	{
		cout << i + 1 << ". " << courses[i] << endl;
	}

	int courseIndex;
	cin >> courseIndex;
	if (courseIndex < 1 || courseIndex > courses.size())
	{
		cout << "Error! Bad course index, check it!";
		return false;
	}

	course = courses[courseIndex - 1];
	return true;
}