#pragma once

#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include "bar_chart.hpp"

using IniMap = std::map<std::string, std::map<std::string, std::string>>;

IniMap parseIni(const std::string& iniFileName);

bool readFile(const std::string& fileName, Database& db, const IniMap& config);