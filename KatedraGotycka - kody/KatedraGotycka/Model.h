#pragma once

#include <string>
#include <map>
#include "ModelFragment.h"

class Model
{
public:
	std::string name;
	std::map<std::string, ModelFragment*> fragments;
	Model(std::string _name) : name(_name) {}
};

