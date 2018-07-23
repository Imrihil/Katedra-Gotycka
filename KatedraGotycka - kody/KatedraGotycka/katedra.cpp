#include <cstdio>
#include "GL\glew.h"
#include "GL\glut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "shaderprogram.h"
#include "ResourceLoader.h"
#include "Mesh.h"
#include "ModelFragmentImporter.h"
#include "ModelImporter.h"
#include "MaterialImporter.h"
#include "tga.h"
#include "SceneData.h"
#include "ShaderTypes.h"
#include "ShaderBuilder.h"

glm::vec3 lightPos(0.0f, 15.0f, 50.0f);
glm::vec3 lightPos2(0.0f, 30.0f, 0.0f);

std::vector<Mesh*> meshes;

int lastTime = 0;
int startTime;
float speed_x = 0.0f, speed_y = 0.0f, speed_z = 0.0f;
float cameraSpeedX = 0.0f, cameraSpeedY = 0.0f, cameraSpeed = 1.0f;

float kadzidloAngle = 0.0f;

glm::vec3 cameraDirection(1.0f, 0.0f, 0.0f);
glm::vec3 cameraPosition(0.0f, 2.0f, 52.0f);
glm::vec3 cameraNose(0.0f, 1.0f, 0.0f);

SceneData *sceneData = new SceneData(3);

int mouse_x, mouse_y;

bool ESCAPE = false;

float cameraVerticalAngle = -0.2f, cameraHorizontalAngle = 1.57f;

int windowWidth = 800;
int windowHeight = 800;

std::map<std::string, Material> materials;

std::map<ShaderType, ShaderProgram*> shaders;

struct Triangle {
	glm::vec3 a, b, c;
	Triangle(glm::vec3 _a, glm::vec3 _b, glm::vec3 _c) : a(_a), b(_b), c(_c) {}
};

vector<Triangle> allowedAreas;

float sign(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    return (p1.x - p3.x) * (p2.z - p3.z) - (p2.x - p3.x) * (p1.z - p3.z);
}

bool isPointInTriangle(Triangle *triangle, glm::vec3 *point) {
	bool b1, b2, b3;

	b1 = sign(*point, triangle->a, triangle->b) < 0.0f;
	b2 = sign(*point, triangle->b, triangle->c) < 0.0f;
	b3 = sign(*point, triangle->c, triangle->a) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

bool isDestinationAllowed(glm::vec3 destination) {
	for (auto area : allowedAreas) {
		if (isPointInTriangle(&area, &destination)) return true;
	}
	return false;
}

ShaderBuilder buildPlainPhongShader() {
	ShaderBuilder shaderBuilder;

	shaderBuilder.enableMMatrix();
	shaderBuilder.enableVMatrix();
	shaderBuilder.enablePMatrix();

	shaderBuilder.enableObserverPosition();
	shaderBuilder.setLightSourceCount(3);
	shaderBuilder.enableNormalVector();
	shaderBuilder.enableTransparencyValue();

	shaderBuilder.enableAmbientMaterial();
	shaderBuilder.enableDiffuseMaterial();
	shaderBuilder.enableSpecularMaterial();

	shaderBuilder.enableAmbientShading();
	shaderBuilder.enableColorDiffuseShading();
	shaderBuilder.enableColorSpecularShading();

	return shaderBuilder;
}

ShaderBuilder buildTexturedDiffusePhongShader() {
	ShaderBuilder shaderBuilder;

	shaderBuilder.enableMMatrix();
	shaderBuilder.enableVMatrix();
	shaderBuilder.enablePMatrix();

	shaderBuilder.enableObserverPosition();
	shaderBuilder.setLightSourceCount(3);
	shaderBuilder.enableNormalVector();
	shaderBuilder.enableTransparencyValue();

	shaderBuilder.enableTextureCoordinates();

	shaderBuilder.enableAmbientMaterial();
	shaderBuilder.enableSpecularMaterial();

	shaderBuilder.enableDiffuseMapTexturingUnit();

	shaderBuilder.enableAmbientShading();
	shaderBuilder.enableTextureDiffuseShading();
	shaderBuilder.enableColorSpecularShading();

	return shaderBuilder;
}

ShaderBuilder buildTexturedDiffuseSpecularPhongShader() {
	ShaderBuilder shaderBuilder;

	shaderBuilder.enableMMatrix();
	shaderBuilder.enableVMatrix();
	shaderBuilder.enablePMatrix();

	shaderBuilder.enableObserverPosition();
	shaderBuilder.setLightSourceCount(3);
	shaderBuilder.enableNormalVector();
	shaderBuilder.enableTransparencyValue();

	shaderBuilder.enableTextureCoordinates();

	shaderBuilder.enableAmbientMaterial();
	shaderBuilder.enableSpecularMapTexturingUnit();
	shaderBuilder.enableDiffuseMapTexturingUnit();

	shaderBuilder.enableAmbientShading();
	shaderBuilder.enableTextureDiffuseShading();
	shaderBuilder.enableTextureSpecularShading();

	return shaderBuilder;
}

void setupShaders() {
	ShaderBuilder plainPhongShaderBuilder = buildPlainPhongShader();
	ShaderBuilder texturedDiffusePhongShaderBuilder = buildTexturedDiffusePhongShader();
	ShaderBuilder texturedDiffuseSpecularPhongShaderBuilder = buildTexturedDiffuseSpecularPhongShader();

	shaders[ShaderType::PLAIN_PHONG_SHADING] = new ShaderProgram(plainPhongShaderBuilder.buildVertexShader().c_str(), plainPhongShaderBuilder.buildFragmentShader().c_str());
	shaders[ShaderType::TEXTURED_KD_PHONG_SHADING] = new ShaderProgram(texturedDiffusePhongShaderBuilder.buildVertexShader().c_str(), texturedDiffusePhongShaderBuilder.buildFragmentShader().c_str());
	shaders[ShaderType::TEXTURED_KD_KS_PHONG_SHADING] = new ShaderProgram(texturedDiffuseSpecularPhongShaderBuilder.buildVertexShader().c_str(),
		texturedDiffuseSpecularPhongShaderBuilder.buildFragmentShader().c_str());
}

void draw() {
	for (auto mesh : meshes) {
		if (mesh->getModelName().substr(0, 6) == "Kadzid") {
			sceneData->M = glm::mat4(1.0f);
			sceneData->M = glm::translate(sceneData->M, glm::vec3(0.0f, 42.0f, 0.0f));
			sceneData->M = glm::rotate(sceneData->M, kadzidloAngle, glm::vec3(0.0f, 0.0f, 1.0f));
			sceneData->M = glm::translate(sceneData->M, glm::vec3(0.0f, -42.0f, 0.0f));
		}
		else {
			sceneData->M = glm::mat4(1.0f);
		}
		mesh->draw(sceneData);
	}
}

void displayFrame() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_MULTISAMPLE_ARB);

	cameraDirection = glm::vec3(
		cos(cameraVerticalAngle) * sin(cameraHorizontalAngle),
		sin(cameraVerticalAngle),
		cos(cameraVerticalAngle) * cos(cameraHorizontalAngle)
		);

	sceneData->P = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	sceneData->V = glm::lookAt(cameraPosition, cameraDirection + cameraPosition, cameraNose);
	sceneData->M = glm::mat4(1.0f);

	sceneData->cameraPosition = cameraPosition;
	sceneData->lightSourcesPositions[0] = lightPos;
	sceneData->lightSourcesPositions[1] = lightPos2;
	sceneData->lightSourcesPositions[2] = cameraPosition + glm::vec3(0, 2, 0);

	draw();

	glutSwapBuffers();
}

void nextFrame() {
	int actTime = glutGet(GLUT_ELAPSED_TIME);
	int interval = actTime - lastTime;
	lastTime = actTime;
	lightPos.x += speed_x*interval / 5.0;
	lightPos.y += speed_y*interval / 5.0;
	lightPos.z += speed_z*interval / 5.0;

	glm::vec3 normalizedCameraDirection = normalize(cameraDirection);

	glm::vec3 projectedCameraDirection(normalizedCameraDirection.x, 0.0f, normalizedCameraDirection.z);

	if (projectedCameraDirection.x != 0.0f || projectedCameraDirection.z != 0.0f) {
		projectedCameraDirection = normalize(projectedCameraDirection);
		glm::vec3 destination = cameraPosition + glm::vec3(cameraSpeedX * projectedCameraDirection.x * 0.03f, 0.0f, 0.0f);
		if (isDestinationAllowed(destination)) {
			cameraPosition = destination;
		}
		destination = cameraPosition + glm::vec3(0.0f, 0.0f, cameraSpeedX * projectedCameraDirection.z * 0.03f);
		if (isDestinationAllowed(destination)) {
			cameraPosition = destination;
		}
		destination = cameraPosition + glm::vec3(cameraSpeedY * normalize(glm::cross(projectedCameraDirection, cameraNose)).x * 0.03f, 0.0f, 0.0f);
		if (isDestinationAllowed(destination)) {
			cameraPosition = destination;
		}
		destination = cameraPosition + glm::vec3(0.0f, 0.0f, cameraSpeedY * normalize(glm::cross(projectedCameraDirection, cameraNose)).z * 0.03f);
		if (isDestinationAllowed(destination)) {
			cameraPosition = destination;
		}
	}

	kadzidloAngle = sin((actTime - startTime) / 1000.0f) / 4.0f;

	if (!ESCAPE) {
		cameraHorizontalAngle -= (mouse_x - windowWidth / 2) * 0.001f;
		cameraVerticalAngle -= (mouse_y - windowHeight / 2) * 0.001f;
		if (cameraVerticalAngle > 1.57f) cameraVerticalAngle = 1.57f;
		else if (cameraVerticalAngle < -1.57f) cameraVerticalAngle = -1.57f;
		if (mouse_x != windowWidth / 2 || mouse_y != windowHeight / 2)	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	}

	glutPostRedisplay();
}

void changeSize(int w, int h) {
	glViewport(0, 0, w, h);
	windowWidth = w;
	windowHeight = h;
}

void increaseCameraSpeed() {
	if (cameraSpeed < 4.0f) {
		cameraSpeedX *= 2;
		cameraSpeedY *= 2;
		cameraSpeed *= 2;
	}
}

void decreaseCameraSpeed() {
	if (cameraSpeed > 0.5f) {
		cameraSpeedX /= 2;
		cameraSpeedY /= 2;
		cameraSpeed /= 2;
	}
}

void keyDown(int c, int x, int y) {
	switch (c) {
	case GLUT_KEY_LEFT:
		speed_x = -0.2f;
		break;
	case GLUT_KEY_RIGHT:
		speed_x = 0.2;
		break;
	case GLUT_KEY_HOME:
		speed_y = 0.2f;
		break;
	case GLUT_KEY_END:
		speed_y = -0.2f;
		break;
	case GLUT_KEY_UP:
		speed_z = -0.2f;
		break;
	case GLUT_KEY_DOWN:
		speed_z = 0.2f;
		break;
	}
}

void keyUp(int c, int x, int y) {
	switch (c) {
	case GLUT_KEY_LEFT:
		speed_x = 0;
		break;
	case GLUT_KEY_RIGHT:
		speed_x = 0;
		break;
	case GLUT_KEY_HOME:
		speed_y = 0;
		break;
	case GLUT_KEY_END:
		speed_y = 0;
		break;
	case GLUT_KEY_UP:
		speed_z = 0;
		break;
	case GLUT_KEY_DOWN:
		speed_z = 0;
		break;
	}
}

void keyboardKeyDown(unsigned char c, int x, int y) {
	std::cout << "+";
	switch (c) {
	case 27:
		if (!ESCAPE) {
			glutPositionWindow(0, 0);
			glutReshapeWindow(windowWidth, windowHeight);
		}
		else {
			glutFullScreen();
		}
		ESCAPE = !ESCAPE;
		glutWarpPointer(windowWidth / 2, windowHeight / 2);
		break;
	case 'w':
		cameraSpeedX += cameraSpeed;
		break;
	case 's':
		cameraSpeedX -= cameraSpeed;
		break;
	case 'a':
		cameraSpeedY -= cameraSpeed;
		break;
	case 'd':
		cameraSpeedY += cameraSpeed;
		break;
	case 'q':
		increaseCameraSpeed();
		break;
	case 'e':
		decreaseCameraSpeed();
		break;
	}
}

void keyboardKeyUp(unsigned char c, int x, int y) {
	switch (c) {
	case 'w':
		cameraSpeedX -= cameraSpeed;
		break;
	case 's':
		cameraSpeedX += cameraSpeed;
		break;
	case 'a':
		cameraSpeedY += cameraSpeed;
		break;
	case 'd':
		cameraSpeedY -= cameraSpeed;
		break;
	}
}

void mouseMove(int x, int y) {
	mouse_x = x;
	mouse_y = y;
}

void initializeGLUT(int* pargc, char** argv) {
	glutInit(pargc, argv); 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	glutInitWindowPosition(0, 0);

	glutCreateWindow("Katedra gotycka");

	glutReshapeFunc(changeSize);
	glutDisplayFunc(displayFrame);
	glutIdleFunc(nextFrame);

	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);

	glutKeyboardFunc(keyboardKeyDown);
	glutKeyboardUpFunc(keyboardKeyUp);

	glutPassiveMotionFunc(mouseMove);

	glutSetCursor(GLUT_CURSOR_NONE);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glEnable(GL_DEPTH_TEST);

	glutFullScreen();
}

void initializeGLEW() {
	GLenum err = glewInit();
	if (GLEW_OK != glewInit()) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(GLEW_VERSION));
		exit(1);
	} else {
		fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	}
}

vector<string>* readModelNamesFromFile(string filePath) {
	int numberOfModels;

	ifstream file(filePath);

	file >> numberOfModels;

	vector<string>* modelNames = new vector<string>(numberOfModels);

	for (int i = 0; i < numberOfModels; i++) {
		file >> (*modelNames)[i];
	}

	file.close();

	return modelNames;
}

void loadModels(string path, string fileName) {
	vector<Model*> models;

	vector<string>* modelNames = readModelNamesFromFile(path + "/" + fileName);

	for (auto modelName : *modelNames) {
		Model* model = ModelImporter::importModel((path + "/" + modelName).c_str(), (modelName + ".model").c_str());
		models.push_back(model);
	}

	delete modelNames;

	for (auto model : models) {
		if (model->name.substr(0, 11) == "Zwiedzianie") {
			for (auto modelFragment : model->fragments) {
				float *tab = modelFragment.second->vertices;
				for (int i = 0; i < modelFragment.second->vertexCount/3; i++) {
					allowedAreas.push_back(Triangle(
						glm::vec3(tab[i * 12], tab[i * 12 + 1], tab[i * 12 + 2]),
						glm::vec3(tab[i * 12 + 4], tab[i * 12 + 5], tab[i * 12 + 6]),
						glm::vec3(tab[i * 12 + 8], tab[i * 12 + 9], tab[i * 12 + 10])));
				}
			}
			continue;
		}
		for (auto modelFragment : model->fragments) {
			Mesh* mesh;
			if (materials[modelFragment.first].map_Kd != "" && modelFragment.second->texCoordSize > 0 && materials[modelFragment.first].map_Ks != "") {
				mesh = new Mesh(modelFragment.second, &materials[modelFragment.first], shaders[ShaderType::TEXTURED_KD_KS_PHONG_SHADING], model->name);
			}
			else if (materials[modelFragment.first].map_Kd != "" && modelFragment.second->texCoordSize > 0) {
				mesh = new Mesh(modelFragment.second, &materials[modelFragment.first], shaders[ShaderType::TEXTURED_KD_PHONG_SHADING], model->name);
			}
			else {
				mesh = new Mesh(modelFragment.second, &materials[modelFragment.first], shaders[ShaderType::PLAIN_PHONG_SHADING], model->name);
			}
			mesh->setupMemory();
			mesh->assignVAOToShader("vertex", "color", "normal", "textureCoordinates");
			meshes.push_back(mesh);
		}
	}

}

int main(int argc, char** argv) {
	initializeGLUT(&argc, argv);
	initializeGLEW();
	setupShaders();

	string path = "models", modelFile = (argc > 1) ? argv[1] : "Katedra.obj.group";

	materials = MaterialImporter::loadMtlFile("models/Katedra.mtl");

	loadModels(path, modelFile);

	sceneData->lightSourcesAmbientValues[0] = 0.4f*glm::vec3(1, 1, 1);
	sceneData->lightSourcesDiffuseValues[0] = 0.4f*glm::vec3(1, 1, 1);
	sceneData->lightSourcesSpecularValues[0] = 0.4f*glm::vec3(1, 1, 1);

	sceneData->lightSourcesAmbientValues[1] = 0.4f*glm::vec3(1, 1, 1);
	sceneData->lightSourcesDiffuseValues[1] = 0.4f*glm::vec3(1, 1, 1);
	sceneData->lightSourcesSpecularValues[1] = 0.4f*glm::vec3(1, 1, 1);

	sceneData->lightSourcesAmbientValues[2] = 0.3f*glm::vec3(1, 1, 1);
	sceneData->lightSourcesDiffuseValues[2] = 0.3f*glm::vec3(1, 1, 1);
	sceneData->lightSourcesSpecularValues[2] = 0.3f*glm::vec3(1, 1, 1);

	cout << allowedAreas.size() << endl;

	startTime = glutGet(GLUT_ELAPSED_TIME);

	glutMainLoop();

	return 0;
}