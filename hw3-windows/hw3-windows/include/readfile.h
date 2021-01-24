// Readfile definitions 
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h" 
#include <glm/gtx/string_cast.hpp>

void rightmultiply (const glm::mat4 & M, std::stack<glm::mat4> &transfstack) ;
bool readvals (std::stringstream &s, const int numvals, float * values) ;
void readfile (const char * filename) ;
