#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "GL/glew.h"
#include "stdio.h"
#include "SceneData.h"
#include "glm\gtc\type_ptr.hpp"

class ShaderProgram {
private:
	GLuint shaderProgram; //Uchwyt reprezentujący program cieniujacy
	GLuint vertexShader; //Uchwyt reprezentujący vertex shader
	GLuint geometryShader; //Uchwyt reprezentujący geometry shader
	GLuint fragmentShader; //Uchwyt reprezentujący fragment shader
	char* readFile(char* fileName); //metoda wczytująca plik tekstowy do tablicy znaków
	GLuint loadShader(GLenum shaderType, char* fileName); //Metoda wczytuje i kompiluje shader, a następnie zwraca jego uchwyt
	GLuint loadShader(GLenum shaderType, const char* data); //Metoda wczytuje i kompiluje shader, a następnie zwraca jego uchwyt
public:
	ShaderProgram(const char* vShaderCode, const char* fShaderCode);
	~ShaderProgram();
	void use(); //Włącza wykorzystywanie programu cieniującego
	GLuint getUniformLocation(char* variableName); //Pobiera numer slotu związanego z daną zmienną jednorodną
	GLuint getAttribLocation(char* variableName); //Pobiera numer slotu związanego z danym atrybutem
	void passUniformSceneData(SceneData* sceneData);
};

#endif