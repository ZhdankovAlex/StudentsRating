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

#include "GroupResult.h"
#include "RatingStrategy.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

class AverageStrategy : public RatingStrategy 
{
public:
	virtual ~AverageStrategy() 
	{
	
	}

	virtual double Result(const GroupResult& studentRating, int studentIdentity);
};