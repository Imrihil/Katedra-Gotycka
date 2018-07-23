#pragma once

#include "Model.h"
#include <iostream>
#include <map>
#include <vector>
#include "Material.h"

struct face {
	unsigned int vertices[3];
	unsigned int texCoords[3];
	unsigned int normals[3];
};

static class ResourceLoader
{
private:
	static std::string readObjectFile(const char* filename);
public:
	static std::vector<Model*> loadModel(const char* fileName);
	static ModelFragment* extractRawDataFromStorage(std::vector<face>&, std::vector<glm::vec3>&, std::vector<glm::vec2>&, std::vector<glm::vec3>&, bool);
};

