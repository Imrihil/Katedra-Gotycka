#pragma once
#include <iostream>
#include <fstream>
#include "Model.h"
#include "ModelFragmentImporter.h"

using namespace std;

class ModelImporter
{
public:
	static Model* importModel(const char* path, const char* modelName);
};

