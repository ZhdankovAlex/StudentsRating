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

bool AddStudent(string& group, string& studentName)
{
	if (group.empty())
	{
		if (!ChooseGroup(group))
		{
			return false;
		}
	}

	if (studentName.empty())
	{
		cout << "--> Input student name: ";
		do
		{
			getline(cin, studentName);
		} while (studentName.empty());

	}

	Group groupLocal;
	if (!groupLocal.Read(group, pathToBaseFolder))
	{
		groupLocal.name = group;
	}

	Student studentLocal;
	studentLocal.name = studentName;
	studentLocal.id = groupLocal.students.size() + 1;

	groupLocal.students.push_back(studentLocal);
	return groupLocal.Write(pathToBaseFolder);
}