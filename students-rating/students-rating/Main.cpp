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
#include "Criterion.h"
#include "Commands.h"

#include "ResultComparator.h"

#include "ChooseCourse.h"
#include "ChooseGroup.h"
#include "ChooseStudent.h"

#include "AddStudent.h"
#include "EditStudent.h"
#include "AddCourse.h"
#include "AddProgress.h"
#include "DeleteStudent.h"
#include "SplitString.h"
#include "Report.h"

#include "PathToBaseFolder.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

int main(int argc, char* argv[])
{
	Commands command = commandNone;
	string criterion;
	string group;
	string studentName;
	string course;
	string date;
	string sort;
	int studentID = 0;
	bool flagTable = false;

	unordered_map<string, int> CriterionsMap;
	CriterionsMap["--add-student"] = 0;
	CriterionsMap["-ads"] = 0;

	CriterionsMap["--edit-name"] = 1;
	CriterionsMap["-enm"] = 1;

	CriterionsMap["--add-course"] = 2;
	CriterionsMap["-acr"] = 2;

	CriterionsMap["--add-progress"] = 3;
	CriterionsMap["-apr"] = 3;

	CriterionsMap["--dlt-student"] = 4;
	CriterionsMap["-dst"] = 4;

	CriterionsMap["--report"] = 5;

	CriterionsMap["--group"] = 6;
	CriterionsMap["-g"] = 6;

	CriterionsMap["--student-name"] = 7;
	CriterionsMap["-n"] = 7;

	CriterionsMap["--student-id"] = 8;
	CriterionsMap["-id"] = 8;

	CriterionsMap["--course"] = 9;
	CriterionsMap["-c"] = 9;

	CriterionsMap["--criterion"] = 10;

	CriterionsMap["--date"] = 11;
	CriterionsMap["-d"] = 11;

	CriterionsMap["--table"] = 12;

	CriterionsMap["--sort"] = 13;

	CriterionsMap["--help"] = 14;
	CriterionsMap["-h"] = 14;

	for (int i = 1; i < argc; i++)
	{
		const char* cp = argv[i];

		if (CriterionsMap.find(cp) != CriterionsMap.end())
		{
			switch (CriterionsMap[cp])
			{
				case 0:
					command = commandAddStudent;
					break;
				case 1:
					command = commandEditStudent;
					break;
				case 2:
					command = commandAddCourse;
					break;
				case 3:
					command = commandAddProgress;
					break;
				case 4:
					command = commandDeleteStudent;
					break;
				case 5:
					command = commandReport;
					break;
				case 6:
					if (i < argc - 1)
					{
						i++;
						group = argv[i];
					}
					else
					{
						cout << "No group name ";
						return -1;
					}
					break;
				case 7:
					if (i < argc - 1)
					{
						i++;
						studentName = argv[i];
					}
					else
					{
						cout << "No student name ";
						return -1;
					}
					break;
				case 8:
					if (i < argc - 1)
					{
						i++;
						studentID = atoi(argv[i]);
					}
					else
					{
						cout << "No student ID ";
						return -1;
					}
					break;
				case 9:
					if (i < argc - 1)
					{
						i++;
						course = argv[i];
					}
					else
					{
						cout << "No course ";
						return -1;
					}
					break;
				case 10:
					if (i < argc - 1)
					{
						i++;
						criterion = argv[i];
					}
					else
					{
						cout << "No criterion ";
						return -1;
					}
					break;
				case 11:
					if (i < argc - 1)
					{
						i++;
						date = argv[i];
					}
					else
					{
						cout << "No date ";
						return -1;
					}
					break;
				case 12:
					flagTable = true;
					break;
				case 13:
					if (i < argc - 1)
					{
						i++;
						sort = argv[i];
					}
					else
					{
						cout << "No sort ";
						return -1;
					}
					break;
				case 14:
					command = commandHelp;
					break;
			} 
		}
		else
		{
			cout << " Error! Something is going wrong... " << endl
				<< " Check the input data! " << endl;
		}
	}

	switch (command)
	{
	case commandHelp:
#pragma region Help
		cout << "ATTENTION! PLEASE READ THE INSTRUCTIONS CAREFULLY!" << endl << endl;
		cout << "Press any key to continue: " << endl;
		cin.get();
		system("cls");
		cout << "--course<COURSE> --criterion<PLUS> --table --sort<AVERAGE=ASC>" << endl << endl << "--report:" << endl << endl
			<< "Build a course report using one or more criteria." << endl
			<< "The report should be built in the form of a text table," << endl
			<< "if the  [--table]  flag is specified:" << endl
			<< "the first column is the student's name," << endl
			<< "the second column is the average rating according to three criteria," << endl
			<< "the other columns are each criterion separately." << endl << endl
			<< "If the flag is not specified, the rating is displayed as a numbered list," << endl
			<< "in parentheses indicates the criterion and rating for each student." << endl << endl
			<< "If the [--sort] flag is specified," << endl
			<< "the list/table is sorted by the specified fields" << endl 
			<< "and in the specified order (desc - descending, asc - ascending)." << endl << endl
			<< "If the sorting option is not specified," << endl
			<< "the sorting is performed by the average rating in descending order." << endl << endl;
		cout << "Press any key to continue: " << endl
			<< "Page 1/6" << endl;
		cin.get();
		system("cls");
		cout << "--course<COURSE> --criteria<TEST> --student-name<STUDENT> --date<DATE>" << endl << endl << "--add-progress:" << endl << endl
			<< "adds info to the journal that a student attended a class on the subject COURSE." << endl << endl
			<< "If [--criteria] is omitted, the default is attendance." << endl
			<< "If [--course] is omitted, the program prompts you to select an item" << endl
			<< "from the list by specifying a number, the list must be sorted alphabetically." << endl << endl
			<< "If [--student-name] is omitted," << endl
			<< "the program prompts you to select a student from the list." << endl << endl
			<< "If [--date] is omitted, the current date is taken as the date of the lesson." << endl << endl;
		cout << "Press any key to continue: " << endl
			<< "Page 2/6" << endl;
		cin.get();
		system("cls");
		cout << "--group<GROUP> --student-name<STUDENT> --course<COURSE>" << endl << endl << "--add-student:" << endl << endl
			<< "adds the specified student to the group and course." << endl
			<< "If the parameters are omitted, the program must interactively ask for data." << endl;
		cout << "Press any key to continue: " << endl
			<< "Page 3/6" << endl;
		cin.get();
		system("cls");
		cout << "--course<COURSE>" << endl << endl << "--add-course:" << endl << endl
			<< "adds a course with the specified name," << endl
			<< "after adding the course offers to" << endl
			<< "interactively add a group and students." << endl;
		cout << "Press any key to continue: " << endl
			<< "Page 4/6" << endl;
		cin.get();
		system("cls");
		cout << "--course<COURSE> --group<GROUP> --student-name<STUDENT>" << endl << endl << "--dlt-student:" << endl << endl
			<< "deletes the specified student from the group and course." << endl
			<< "If the parameters are omitted, the program must interactively ask for data." << endl;
		cout << "Press any key to continue: " << endl
			<< "Page 5/6" << endl;
		cin.get();
		system("cls");
		cout << "--group<GROUP> --student-id<ID>" << endl << endl << "--edit-name:" << endl << endl
			<< "replaces the specified student from the group by ID." << endl
			<< "If the parameters are omitted, the program must interactively ask for data." << endl;
		cout << "Press any key to continue: " << endl
			<< "Page 6/6" << endl;
		cin.get();
		system("cls");
#pragma endregion
		return -1;
		break;
	case commandNone:
		cout << "No command! Try --help" << endl;
		break;
	case commandAddStudent:
		AddStudent(group, studentName);
		break;
	case commandEditStudent:
		EditStudent(group, studentID);
		break;
	case commandAddCourse:
		AddCourse(course);
		break;
	case commandAddProgress:
		AddProgress(course, criterion, group, studentName, date); //
		break;
	case commandDeleteStudent:
		DeleteStudent(course, group, studentName);
		break;
	case commandReport:
		Report(course, criterion, flagTable, sort); //
		break;
	}
	return 0;
}