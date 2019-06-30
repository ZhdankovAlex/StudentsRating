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

#include "TestStrategy.h"
#include "GroupResult.h"
#include "StudentRating.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

double TestStrategy::Result(const GroupResult& studentsRating, int studentIdentity) 
{
	double sum = 0;
	set<string> uniqDates;
	
	for (map<int, StudentRating>::const_iterator i = studentsRating.progress.begin();
		i != studentsRating.progress.end(); i++)
	{
		
		for (StudentRating::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			uniqDates.insert(j->first);
			if (i->first == studentIdentity)
			{
				if (j->second.test) 
				{
					sum += j->second.test;
				}
			}
		}
	}

	if (uniqDates.size() == 0)
	{
		return 0;
	}
	else
	{
		return (sum / (5 * uniqDates.size()));
	}
}