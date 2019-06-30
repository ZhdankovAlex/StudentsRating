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

#include "Course.h"
#include "Student.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

bool Course::RemoveResult(const string& groupName, int studentIdentity) 
{
	for (iterator i = begin(); i != end(); i++)
	{
		if (i->group.compare(groupName) == 0) 
		{
			i->progress.erase(studentIdentity);
			if (i->progress.size() == 0) 
			{
				erase(i);
			}
			return true;
		}
	}

	return false;
}

void Course::List(vector<string>* course, const string& pathToBaseFolder) 
{
	string fileName(pathToBaseFolder + "\\course\\*.json");

#ifdef WIN32
	HANDLE hFind;
	WIN32_FIND_DATAA FindFileData;
	if ((hFind = FindFirstFileA(fileName.c_str(), &FindFileData)) != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			char *cp = strrchr(FindFileData.cFileName, '.');
			*cp = 0;
			course->push_back(FindFileData.cFileName);
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
		course->push_back(dirp->d_name);
	}
	closedir(dp);
#endif
}

bool Course::Read(const string& name, const string& pathToBaseFolder)
{
	bool returnLocal = true;
	try 
	{
		this->name = name;
		string fileName(pathToBaseFolder + "\\course\\" + name + ".json");

		ifstream input(fileName);

		if (!input.is_open())
		{
			cout << "Can not open file" << endl;
			returnLocal = false;
		} 
		else
			if (input.peek() == EOF)
			{
				cout << "File is empty" << endl;
				returnLocal = false;
			}

		json j;   
		input >> j;
		
		for (json::iterator it = j.begin(); it != j.end(); ++it) 
		{
			GroupResult groupLocal(*it); 
			this->push_back(groupLocal);
		}
	}
	catch (...) 
	{
		returnLocal = false;
	}

	return returnLocal;
}

bool Course::Write(const string& pathToBaseFolder) 
{
	json courseJson;

	for (vector<GroupResult>::const_iterator i = begin(); i != end(); i++) //
	{
		json groupJson;
		groupJson["group"] = i->group;

		json progressJson;
	
		for (map<int, StudentRating>::const_iterator j = i->progress.begin();
			j != i->progress.end(); j++)
		{
			char studentNum[20];
			sprintf(studentNum, "%d", j->first);

			json studentJson;
			
			for (StudentRating::const_iterator l = j->second.begin();
				l != j->second.end(); l++)
			{
				json descriptionJson;
				if (l->second.attendance) 
				{
					descriptionJson["attendance"] = true;
				}
				if (l->second.plus) 
				{
					descriptionJson["plus"] = true;
				}
				if (l->second.test != 0.0) 
				{
					descriptionJson["test"] = l->second.test;
				}
				studentJson[l->first] = descriptionJson;
			}
			progressJson[studentNum] = studentJson;
		}

		groupJson["progress"] = progressJson;

		courseJson.push_back(groupJson);
	}

#ifdef WIN32
	mkdir(pathToBaseFolder.c_str());
#else
	mkdir(pathToBaseFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

	string folder(pathToBaseFolder + "\\course");

#ifdef WIN32
	mkdir(folder.c_str());
#else
	mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

	ofstream output(folder + "\\" + name + ".json");
	output << setw(4) << courseJson.dump(4) << endl;
	return true;
}