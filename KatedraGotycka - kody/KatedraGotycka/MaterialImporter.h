#pragma once
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Material.h"

using namespace std;

class MaterialImporter
{
public:
	static map<string, Material> loadMtlFile(const char* filename);
	static Material readMaterial(ifstream *in);
};

