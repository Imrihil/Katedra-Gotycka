#pragma once

#include <string>
#include "GL\glew.h"
#include "Model.h"
#include "shaderprogram.h"
#include "Material.h"
#include "ModelFragment.h"

class Mesh
{
private:
	std::string modelName;
	ModelFragment* modelFragment;
	Material* material;
	ShaderProgram* shaderProgram;

	GLuint verticiesVBO;
	GLuint colorsVBO;
	GLuint normalsVBO;
	GLuint texCoordsVBO;

	GLuint VAO;

public:
	Mesh(ModelFragment* _modelFragment, Material* _material, ShaderProgram* _shaderProgram, std::string _modelName);
	void setupMemory();
	void assignVAOToShader(char* vertexAttributeName = nullptr, char* colorAttributeName = nullptr, char* normalAttributeName = nullptr, char* texCoordAttributeName = nullptr);
	void draw(SceneData* sceneData);
	std::string getModelName();
	virtual ~Mesh();
};

