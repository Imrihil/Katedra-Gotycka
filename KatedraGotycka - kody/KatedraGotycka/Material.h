#pragma once

#include <string>
#include "glm\glm.hpp"

class Material
{
public:
	std::string name;
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	std::string map_Kd;
	std::string map_Ks;
	std::string map_d;
	unsigned int diffuseTextureHandle;
	unsigned int specularTextureHandle;
	float Ns;
	float Ni;
	float d;
	int illum;
};

