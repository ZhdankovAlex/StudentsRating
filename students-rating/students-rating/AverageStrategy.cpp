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

#include "AverageStrategy.h"
#include "AttendanceStrategy.h"
#include "PlusStrategy.h"
#include "TestStrategy.h"
#include "GroupResult.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

double AverageStrategy::Result(const GroupResult& studentRating, int studentIdentity) 
{
	AttendanceStrategy strategyAttendance;
	PlusStrategy strategyAddition;
	TestStrategy strategyTest;

	double result = strategyAttendance.Result(studentRating, studentIdentity);
	result += strategyAddition.Result(studentRating, studentIdentity);
	result += strategyTest.Result(studentRating, studentIdentity);
	return result / 3;
}