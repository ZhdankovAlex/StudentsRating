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

#include "SplitString.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

bool AddProgress(string& courseName, string& criterionName, string& groupName, string& studentName, string& date)
{
	Criterion criterion = criterionAttendance;
	vector<string> criterionParts;

	unordered_map<string, Criterion> MarksCriterion;
	MarksCriterion["attendance"] = criterionAttendance;
	MarksCriterion["plus"] = criterionPlus;
	MarksCriterion["test"] = criterionTest;

	if (criterionName.empty()) //
	{
		cout << "--> Input the criterion: " << endl;
		cin >> criterionName;
	}

	SplitString(&criterionParts, criterionName, ',');

	for (int i = 0; i < criterionParts.size(); i++)
	{
		if (MarksCriterion.find(criterionParts[i]) != MarksCriterion.end())
		{
			switch (MarksCriterion[criterionParts[i]])
			{
			case criterionAttendance:
				criterion = criterionAttendance;
				break;
			case criterionPlus:
				criterion = criterionPlus;
				break;
			case criterionTest:
				criterion = criterionTest;
				break;
			}
		}

		switch (criterion)
		{
		case criterionAttendance:
			cout << "--> Attendance" << endl;
			break;
		case criterionPlus:
			cout << "--> Plus" << endl;
			break;
		case criterionTest:
			cout << "--> Test" << endl;
			break;
		}

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

		
		if (date.empty())
		{
			cout << "--> Input date" << endl;
			cin >> date;
		}
		if (date == "today" || date == "Today")
		{
			time_t currentTime = time(NULL);
			tm* tmp = localtime(&currentTime);
			char buf[20];
			sprintf(buf, "%d/%02d/%02d", tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday);
			date = buf;
		}

		GroupResult& groupResult = course.FindOrInsert(group.name);
		StudentRating& studentRating = groupResult.FindOrInsert(student.id);
		DailyRating& dailyRating = studentRating.FindOrInsert(date);

		switch (criterion)
		{
		case criterionTest:
			double value;
			cout << "--> Input the value of the test: ";
			cin >> value;
			if (value > 5)
			{
				cout << "Error! Value must not exceed 5.0! Try again!" << endl;
				break;
			}
			dailyRating.test = value;
			dailyRating.attendance = true;
			break;
		case criterionAttendance:
			dailyRating.attendance = true;
			break;
		case criterionPlus:
			dailyRating.plus = true;
			dailyRating.attendance = true;
			break;
		}
		course.Write(pathToBaseFolder);
		cout << "Press any key to continue: " << endl;
		cin.get();
		system("cls");
	}
	return true;
}