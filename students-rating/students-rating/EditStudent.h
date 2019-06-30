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

bool EditStudent(string& group, int studentIdentity)
{
	if (group.empty())
	{
		if (!ChooseGroup(group))
		{
			return false;
		}
	}

	Group groupLocal;
	if (!groupLocal.Read(group, pathToBaseFolder))
	{
		cout << "No group";
		return false;
	}

	int index = studentIdentity - 1;
	if (index == 0 && !ChooseStudent(index, groupLocal))
	{
		return false;
	}

	Student &currentStudent = groupLocal.students[index];
	cout << "--> Input the new name :";

	string name;
	do
	{
		getline(cin, name);
	} while (name.empty());

	currentStudent.name = name;
	groupLocal.Write(pathToBaseFolder);
	return true;
}