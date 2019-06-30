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

#include "SplitString.h"

#include "PathToBaseFolder.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

bool Report(string& courseName, string& criterionName, bool flagTable, string& sortType)
{
	Course course;
	if (courseName.empty())
	{
		if (!ChooseCourse(courseName))
		{
			return false;
		}
	}

	course.Read(courseName, pathToBaseFolder);

	AttendanceStrategy attendanceStrategy;
	PlusStrategy plusStrategy;
	TestStrategy testStrategy;
	AverageStrategy averageStrategy;

	vector<string> outColumns;
	vector<int> outCriterion;
	vector<StudentResult> results;

	vector<string> sortColumns;
	vector<SortType> sortingType;

	if (criterionName.empty())
	{
		criterionName = "average";
	}

	SplitString(&outColumns, criterionName, ',');

	unordered_map<string, int> Marks;
	Marks["attendance"] = 0;
	Marks["plus"] = 1;
	Marks["test"] = 2;
	Marks["average"] = 3;

	unordered_map<string, int> Sorts;
	Sorts["attendance"] = 0;
	Sorts["plus"] = 1;
	Sorts["test"] = 2;
	Sorts["average"] = 3;

	for (int i = 0; i < outColumns.size(); i++)
	{
		if (Marks.find(outColumns[i]) != Marks.end())
		{
			switch (Marks[outColumns[i]])
			{
			case 0:
				outCriterion.push_back(criterionAttendance);
				break;
			case 1:
				outCriterion.push_back(criterionPlus);
				break;
			case 2:
				outCriterion.push_back(criterionTest);
				break;
			case 3:
				outCriterion.push_back(criterionAverage);
				break;
			default:
				cout << "No criterion named by" << outColumns[i];
				return false;
				break;
			}
		}
	}

	if (sortType.empty())
	{
		SortType sr;
		sr.ascending = false;
		sr.criterion = criterionAverage;
		sortingType.push_back(sr);
	}
	else
	{
		SplitString(&sortColumns, sortType, ',');

		for (int i = 0; i < sortColumns.size(); i++)
		{
			vector<string> sortParts;

			SplitString(&sortParts, sortColumns[i], '='); 
			if (sortParts.size() != 2)
			{
				cout << "Error. You must check sort options!" << endl;
				return false;
			}

			SortType sortT;
			
			if (Sorts.find(sortParts[0]) != Sorts.end())
			{
				switch (Sorts[sortParts[0]])
				{
					case 0:
						sortT.criterion = criterionAttendance;
						break;
					case 1:
						sortT.criterion = criterionPlus;
						break;
					case 2:
						sortT.criterion = criterionTest;
						break;
					case 3:
						sortT.criterion = criterionAverage;
						break;
					default:
						cout << "No criterion named by " << sortParts[i];
						return false;
						break;
				}
			}

			sortT.ascending = (sortParts[1].compare("ascending") == 0);
			if (sortT.ascending == false)
			{
				sortT.ascending = (sortParts[1].compare("asc") == 0);
			}

			sortingType.push_back(sortT);
		}
	}

	for (Course::const_iterator j = course.begin(); j != course.end(); j++) 
	{
		Group groupLocal;
		groupLocal.Read(j->group, pathToBaseFolder);

		for (map<int, StudentRating>::const_iterator l = j->progress.begin();
			l != j->progress.end(); l++)
		{
			int studentIndex = groupLocal.FindStudent(l->first);
			if (studentIndex > 0)
			{
				studentIndex--;

				StudentResult sres;
				sres.name = groupLocal.students[studentIndex].name;
				sres.result[0] = attendanceStrategy.Result(*j, l->first);
				sres.result[1] = plusStrategy.Result(*j, l->first);
				sres.result[2] = testStrategy.Result(*j, l->first);
				sres.result[3] = averageStrategy.Result(*j, l->first);

				results.push_back(sres);
			}
		}
	}

	ResultComparator resCmp(sortingType);
	std::sort(results.begin(), results.end(), resCmp);

	if (flagTable)
	{
		cout << "FIO:" << "			" << "Averg:" << "	";
		for (int j = 0; j < outCriterion.size(); j++)
		{
			if (outCriterion[j] == 0)
			{
				cout << "Att:" << "	";
			}
			if (outCriterion[j] == 1)
			{
				cout << "Plus:" << "	";
			}
			if (outCriterion[j] == 2)
			{
				cout << "Test:" << "	";
			}
		}
		cout << endl;
		for (int i = 0; i < results.size(); i++)
		{
			cout << setprecision(3) << fixed << results[i].name << "    " << '	' << results[i].result[criterionAverage];
			for (int j = 0; j < outCriterion.size(); j++)
			{
				cout << setprecision(3) << fixed << '	' << results[i].result[(Criterion)(outCriterion[j])];
			}
			cout << endl;
		}
	}
	else
	{
		int index = 1;

		ResultComparator resCmp(sortingType);
		std::sort(results.begin(), results.end(), resCmp);

		cout << "Sort by average:" << endl;

		for (int j = 0; j < results.size(); j++) //
		{
			cout << index << ". " << results[j].name << " " << '	' << " (";
			bool powerOn = true;
			int l = 0;
			for (int i = 0; i < outCriterion.size(); i++) //
			{
				if (powerOn)
				{
					powerOn = false;
				}
				else
				{
					cout << ",  ";
				}
				cout << setprecision(3) << fixed << outColumns[l] << ": " << results[j].result[outCriterion[i]];
				l++;
			}
			cout << ")" << endl;
			index++;
		}
	}

	return true;
}