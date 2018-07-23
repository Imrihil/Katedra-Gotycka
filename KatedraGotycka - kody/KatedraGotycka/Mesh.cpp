#include "Mesh.h"
#include <iostream>

GLuint makeBuffer(void *data, int vertexCount, int vertexSize) {
	GLuint handle;

	glGenBuffers(1, &handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return handle;
}

void assignVBOtoAttribute(ShaderProgram* shaderProgram, char* attributeName, GLuint bufVBO, int variableSize) {
	GLuint location = shaderProgram->getAttribLocation(attributeName);
	glBindBuffer(GL_ARRAY_BUFFER, bufVBO); 
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, variableSize, GL_FLOAT, GL_FALSE, 0, NULL);
}

Mesh::Mesh(ModelFragment* _modelFragment, Material* _material, ShaderProgram* _shaderProgram, std::string _modelName)
	: modelFragment(_modelFragment), material(_material), shaderProgram(_shaderProgram), modelName(_modelName) {}

void Mesh::assignVAOToShader(char* vertexAttributeName, char* colorAttributeName, char* normalAttributeName, char* texCoordAttributeName) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	if (vertexAttributeName != nullptr) assignVBOtoAttribute(shaderProgram, vertexAttributeName, verticiesVBO, modelFragment->vertexSize);
	if (colorAttributeName != nullptr) assignVBOtoAttribute(shaderProgram, colorAttributeName, colorsVBO, modelFragment->colorSize);
	if (normalAttributeName != nullptr) assignVBOtoAttribute(shaderProgram, normalAttributeName, normalsVBO, modelFragment->normalSize);
	if (texCoordAttributeName != nullptr) assignVBOtoAttribute(shaderProgram, texCoordAttributeName, texCoordsVBO, modelFragment->texCoordSize);

	glBindVertexArray(0);
}

void Mesh::setupMemory() {
	if (modelFragment->vertexSize > 0) verticiesVBO = makeBuffer(modelFragment->vertices, modelFragment->vertexCount, modelFragment->vertexSize * sizeof(float));
	if (modelFragment->colorSize > 0) colorsVBO = makeBuffer(modelFragment->colors, modelFragment->vertexCount, modelFragment->colorSize * sizeof(float));
	if (modelFragment->normalSize > 0) normalsVBO = makeBuffer(modelFragment->normals, modelFragment->vertexCount, modelFragment->normalSize * sizeof(float));
	if (modelFragment->texCoordSize > 0) {
		texCoordsVBO = makeBuffer(modelFragment->texCoords, modelFragment->vertexCount, modelFragment->texCoordSize * sizeof(float));
	}
}

void Mesh::draw(SceneData* sceneData) {
	shaderProgram->use();
	shaderProgram->passUniformSceneData(sceneData);

	for (int i = 0; i < sceneData->lightSources; i++) {
		glUniform4f(shaderProgram->getUniformLocation(&("La" + std::to_string(i))[0]), sceneData->lightSourcesAmbientValues[i].r, sceneData->lightSourcesAmbientValues[i].g, sceneData->lightSourcesAmbientValues[i].b, 1);
		glUniform4f(shaderProgram->getUniformLocation(&("Ld" + std::to_string(i))[0]), sceneData->lightSourcesDiffuseValues[i].r, sceneData->lightSourcesDiffuseValues[i].g, sceneData->lightSourcesDiffuseValues[i].b, 1);
		glUniform4f(shaderProgram->getUniformLocation(&("Ls" + std::to_string(i))[0]), sceneData->lightSourcesSpecularValues[i].r, sceneData->lightSourcesSpecularValues[i].g, sceneData->lightSourcesSpecularValues[i].b, 1);
	}

	glUniform4f(shaderProgram->getUniformLocation("Ma"), material->Ka.x, material->Ka.y, material->Ka.z, 1);
	glUniform4f(shaderProgram->getUniformLocation("Md"), material->Kd.x, material->Kd.y, material->Kd.z, 1);
	glUniform4f(shaderProgram->getUniformLocation("Ms"), material->Ks.x, material->Ks.y, material->Ks.z, 1);
	glUniform1f(shaderProgram->getUniformLocation("Ns"), material->Ns);
	glUniform1f(shaderProgram->getUniformLocation("d"), material->d);

	if (material->map_Kd != "") {
		glUniform1i(shaderProgram->getUniformLocation("texture_map_Kd"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->diffuseTextureHandle);
	}

	if (material->map_Ks != "") {
		glUniform1i(shaderProgram->getUniformLocation("texture_map_Ks"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material->specularTextureHandle);
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, modelFragment->vertexCount);
	glBindVertexArray(0);
}

std::string Mesh::getModelName() {
	return modelName;
}

Mesh::~Mesh()
{
	delete modelFragment;
}