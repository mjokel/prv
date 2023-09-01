#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "classes.h"

long findETX(std::ifstream& f);
std::vector<char> getHeader(std::ifstream& f, size_t n);
std::map<std::string, MetaInfo> getMetaInfo();
std::tuple<MetaInfo, std::vector<char>> parse(std::vector<char> b, long i, const std::map<std::string, MetaInfo>& m);
