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

bool ChooseStudent(int& studentIndx, const Group& group)
{
	if (group.students.size() == 0)
	{
		cout << "No any students in group";
		return false;
	}

	if (group.students.size() > 1)
	{
		cout << "--> Choose student (1 - " << group.students.size() << ")" << endl;
	}
	else
	{
		cout << "--> Choose student (only one student)" << endl;
	}

	int i = 0;
	for (int j = 0; j < group.students.size(); j++) //
	{
		cout << i + 1 << ". " << group.students[j].name << endl;
		i++;
	}

	int studentIndex;
	cin >> studentIndex;
	if (studentIndex < 1 || studentIndex > group.students.size())
	{
		cout << "Error! Bad student index, check it!";
		return false;
	}

	studentIndx = studentIndex - 1;
	return true;
}