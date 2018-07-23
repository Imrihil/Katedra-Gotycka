#pragma once

#include "ModelFragment.h"
#include <cstdio>
#include <iostream>

using namespace std;

class ModelFragmentImporter
{
public:
	static ModelFragment* importModelFragment(const char* fileName);
};

