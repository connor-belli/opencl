#pragma once

#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

#include <CL/cl.hpp>

using namespace cl;

Device get_device();

void blur(std::vector<int> & a, std::vector<int> & b, std::vector<int> & c); 
