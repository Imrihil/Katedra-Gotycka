#pragma once

#include "glm\glm.hpp"

class SceneData {
public:
	glm::mat4 M;
	glm::mat4 V;
	glm::mat4 P;
	glm::vec3 cameraPosition;
	glm::vec3 *lightSourcesPositions;
	glm::vec3 *lightSourcesAmbientValues;
	glm::vec3 *lightSourcesDiffuseValues;
	glm::vec3 *lightSourcesSpecularValues;
	unsigned int lightSources;
	SceneData(int _lightSources) : lightSources(_lightSources) {
		lightSourcesPositions = new glm::vec3[lightSources];
		lightSourcesAmbientValues = new glm::vec3[lightSources];
		lightSourcesDiffuseValues = new glm::vec3[lightSources];
		lightSourcesSpecularValues = new glm::vec3[lightSources];
	}
	virtual ~SceneData() {
		delete[] lightSourcesPositions;
	}
};