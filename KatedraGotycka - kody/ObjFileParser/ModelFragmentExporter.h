#pragma once

#include <iostream>
#include <cstdio>
#include "ModelFragment.h"

using namespace std;

class ModelFragmentExporter
{
public:
	static void exportModelFragment(ModelFragment* modelFragment, const char* fileName);
};

