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

#include "PathToBaseFolder.h"

#include "ResultComparator.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

bool AddCourse(string& course)
{
	if (course.empty())
	{
		cout << "--> Input course name: ";
		cin >> course;
	}

	Course courseLocal;
	courseLocal.name = course;
	courseLocal.Write(pathToBaseFolder);

	cout << "--> Do you want input a group? (Y/N) (y/n)" << endl;

	char flag;
	string group;
	string student;
	cin >> flag;
	int count = 0;
	if (flag == 'y' || flag == 'Y')
	{
		cout << "--> Input the name of the group: ";
		cin >> group;
		cout << "--> Input the number of students: ";
		cin >> count;
		for (int i = 0; i < count; i++)
		{
			AddStudent(group, student);
			student = "";
		}
	}
	else
	{
		return false;
	}
	return true;
}