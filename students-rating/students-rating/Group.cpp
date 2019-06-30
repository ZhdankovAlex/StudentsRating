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

#include "Group.h"
#include "Student.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

bool Group::Read(const string& name, const string& pathToBaseFolder) 
{
	bool returnLocal = true;
	try 
	{
		this->name = name;
		string fileName(pathToBaseFolder + "\\groups\\" + name + ".json");

		ifstream input;
		input.open(fileName);
		if (!input) 
		{
			return false;
		}
		json j;
		input >> j;

		json& studentLocal = j["students"];

		for (json::iterator it = studentLocal.begin(); it != studentLocal.end(); ++it)
		{
			Student currentStudent(*it);
			this->students.push_back(currentStudent);
		}

	}
	catch (...) 
	{
		returnLocal = false;
	}
	return returnLocal;
}

bool Group::Write(const string& pathToBaseFolder) const
{
	json studentsWrite;
	
	for (vector<Student>::const_iterator i = students.begin(); i != students.end(); i++)
	{
		json currentStudents;
		currentStudents["id"] = i->id;
		currentStudents["name"] = i->name;
		studentsWrite.push_back(currentStudents);
	}

	json j;
	j["students"] = studentsWrite;

#ifdef WIN32
	mkdir(pathToBaseFolder.c_str());
#else
	mkdir(pathToBaseFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

	string folder(pathToBaseFolder + "\\groups");

#ifdef WIN32
	mkdir(folder.c_str());
#else
	mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

	ofstream output(folder + "\\" + name + ".json");
	output << setw(4) << j << endl;
	return true;
}

int Group::FindStudent(int id) const 
{
	int required = 0;
	
	for (vector<Student>::const_iterator i = students.begin(); 
		 i != students.end(); i++, required++)
	{
		if (i->id == id) 
		{
			return required + 1;
		}
	}
	return required;
}

int Group::FindStudent(const string& studentName) const 
{
	int required = 0;
	
	for (vector<Student>::const_iterator i = students.begin();
		i != students.end(); i++, required++)
	{
		if (i->name.compare(studentName) == 0) 
		{
			return required + 1;
		}
	}
	return required;
}

void Group::List(vector<string>* groups, const string& pathToBaseFolder) 
{

	string fileName(pathToBaseFolder + "\\groups\\*.json");

#ifdef WIN32
	HANDLE hFind;
	WIN32_FIND_DATAA FindFileData;
	if ((hFind = FindFirstFileA(fileName.c_str(), &FindFileData)) != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			char *cp = strrchr(FindFileData.cFileName, '.');
			*cp = 0;
			groups->push_back(FindFileData.cFileName);
		} while (FindNextFileA(hFind, &FindFileData));

		FindClose(hFind);
	}
#else
	DIR * dp;
	struct dirent * dirp;
	if ((dp = opendir(fileName.c_str())) == NULL) 
	{
		return;
	}

	while ((dirp = readdir(dp)) != NULL) 
	{
		char *cp = strrchr(dirp->d_name, '.');
		*cp = 0;
		groups->push_back(dirp->d_name);
	}
	closedir(dp);
#endif
}