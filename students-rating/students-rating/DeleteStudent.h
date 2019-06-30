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

#include "ChooseCourse.h"
#include "ChooseGroup.h"
#include "ChooseStudent.h"

#include "PathToBaseFolder.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

bool DeleteStudent(string& courseName, string& groupName, string& studentName)
{
	Course course;
	Group group;

	if (courseName.empty())
	{
		if (!ChooseCourse(courseName))
		{
			return false;
		}
	}

	course.Read(courseName, pathToBaseFolder);

	if (groupName.empty())
	{
		if (!ChooseGroup(groupName))
		{
			return false;
		}
	}

	group.Read(groupName, pathToBaseFolder);

	int studentIndex = 0;
	if (!studentName.empty())
	{
		studentIndex = group.FindStudent(studentName);
	}
	if (studentIndex == 0)
	{
		if (!ChooseStudent(studentIndex, group))
		{
			return false;
		}
	}
	else
	{
		studentIndex--;
	}
	Student& student = group.students[studentIndex];

	course.RemoveResult(groupName, student.id);
	course.Write(pathToBaseFolder);
	return true;
}