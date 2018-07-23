#pragma once
#include <cstdio>
#include <fstream>
#include "ModelFragmentExporter.h"
#include "Model.h"
#include <Windows.h>

using namespace std;

class ModelExporter
{
public:
	static void exportModel(Model* model, const char* fileName);
};

