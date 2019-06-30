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

#include "SortType.h"

#include "StudentResult.h"

#include "json.hpp"

using namespace std;
using nlohmann::json;

class ResultComparator
{
public:
	ResultComparator(vector<SortType>& _order) : order(_order)
	{

	}

	bool operator() (const StudentResult& left, const StudentResult& right) const
	{
		for (int i = 0; i < order.size(); i++)
		{
			double leftValue = ((StudentResult&)left).result[order[i].criterion];
			double rightValue = ((StudentResult&)right).result[order[i].criterion];

			double result;
			if (order[i].ascending)
			{
				result = leftValue - rightValue;
			}
			else
			{
				result = rightValue - leftValue;
			}

			if (result < 0)
			{
				return true;
			}
			if (result > 0)
			{
				return false;
			}
		}
		return false;
	}

private:
	vector<SortType>& order;
};