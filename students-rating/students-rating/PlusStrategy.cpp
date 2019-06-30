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

#include "PlusStrategy.h"
#include "GroupResult.h"
#include "StudentRating.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

double PlusStrategy::Result(const GroupResult& studentRating, int studentIdentity) 
{
	double plus = 0;
	set<string> uniqDates;

	for (map<int, StudentRating>::const_iterator i = studentRating.progress.begin();
		i != studentRating.progress.end(); i++)
	{
		
		for (StudentRating::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			uniqDates.insert(j->first);
			if (i->first == studentIdentity) 
			{
				if (j->second.plus) 
				{
					plus++;
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
		return (plus / uniqDates.size());
	}
}