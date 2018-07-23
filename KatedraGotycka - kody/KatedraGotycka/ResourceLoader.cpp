#include "ResourceLoader.h"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "glm\glm.hpp"

unsigned int readIndex(std::string &text) {
	while (text[0] == ' ') text = text.substr(1);
	int position = text.find("/");
	if (text.find(" ") != std::string::npos && text.find(" ") < position) position = text.find(" ");
	if (position == -1) position = text.size();
	unsigned int result;
	if (position == 0) result = 0;
	else result	= unsigned int(std::stoi(text.substr(0, position)));
	if (position != text.size()) {
		text = text.substr(position + 1);
	}
	else {
		text = text.substr(position);
	}
	return result;
}

std::vector<glm::vec3> readVertices(std::vector<std::string> &lines, std::string prefix) {
	std::vector<glm::vec3> vertices;
	for (auto line : lines) {
		if (line.substr(0, prefix.size()) == prefix) {
			std::istringstream s(line.substr(prefix.size()));
			glm::vec3 v;
			s >> v.x >> v.y >> v.z;
			vertices.push_back(v);
		}
	}
	return vertices;
}

std::vector<glm::vec2> readTexCoords(std::vector<std::string> &lines) {
	std::vector<glm::vec2> texCoords;
	for (auto line : lines) {
		if (line.substr(0, 3) == "vt ") {
			std::istringstream s(line.substr(3));
			glm::vec2 v;
			s >> v.x >> v.y;
			texCoords.push_back(v);
		}
	}
	return texCoords;
}

std::vector<std::string> readFile(const char* fileName) {

	std::ifstream in(fileName, std::ios::in);
	if (!in) throw "File " + std::string(fileName) + " could not be opened.";

	in.seekg(0, std::ios::end);
	int size = in.tellg();
	std::string buffer(size, ' ');
	in.seekg(0);

	std::cout << "Let's go" << std::endl;
	in.read(&buffer[0], size);
	std::cout << "Go ahead" << std::endl;

	std::stringstream stream(buffer);

	std::vector<std::string> lines;

	std::string line;

	while (getline(stream, line)) {
		lines.push_back(line);
	}

	std::cout << "Done" << std::endl;
	return lines;
}

std::vector<Model*> ResourceLoader::loadModel(const char *fileName)
{
	std::vector<std::string> lines = readFile(fileName);
	std::cout << "Lines: " << lines.size() << std::endl;

	std::vector<glm::vec3> vertices = readVertices(lines, "v ");
	std::cout << "Vertices: " << vertices.size() << std::endl;
	std::vector<glm::vec2> texCoords = readTexCoords(lines);
	std::cout << "TexCoords: " << texCoords.size() << std::endl;
	std::vector<glm::vec3> normals = readVertices(lines, "vn ");
	std::cout << "Normals: " << normals.size() << std::endl;

	std::vector<face> faces;
	std::string material;

	bool texCoordsPresent = true;

	std::vector<Model*> models;
	Model* currentModel = nullptr;
	std::string currentMaterial = "";

	for (auto line : lines) {
		if (line.substr(0, 2) == "o ") {
			if (currentMaterial != "") {
				currentModel->fragments[currentMaterial] = extractRawDataFromStorage(faces, vertices, texCoords, normals, texCoordsPresent);
			}
			if (currentModel != nullptr) models.push_back(currentModel);
			currentModel = new Model(line.substr(2));
			currentMaterial = "";
			faces.clear();
			texCoordsPresent = true;
		}
		else if (line.substr(0, 2) == "f ") {
			if (currentModel == nullptr || currentMaterial == "") throw "There is an uncategorized face!";
			line = line.substr(2);
			face f;
			for (int i = 0; i < 3; i++) {
				f.vertices[i] = readIndex(line) - 1;
				if (line[0] == '/') {
					line = line.substr(1, line.size() - 1);
					texCoordsPresent = false;
				}
				else f.texCoords[i] = readIndex(line) - 1;
				f.normals[i] = readIndex(line) - 1;
			}
			faces.push_back(f);
		}
		else if (line.substr(0, 6) == "usemtl") {
			if (currentModel == nullptr) throw "There is an uncategorized material usage!";
			if (currentMaterial != "") {
				currentModel->fragments[currentMaterial] = extractRawDataFromStorage(faces, vertices, texCoords, normals, texCoordsPresent);
			}
			std::istringstream s(line.substr(6));
			s >> currentMaterial;
			faces.clear();
			texCoordsPresent = true;
		}
		else if (line.substr(0, 2) != "v " && line.substr(0, 3) != "vt " && line.substr(0, 3) != "vn " && line[0] != '#') {
			std::cout << line << "\n";
		}

	}
	std::cout << "Faces: " << faces.size() << std::endl;
	if (currentMaterial != "") {
		currentModel->fragments[currentMaterial] = extractRawDataFromStorage(faces, vertices, texCoords, normals, texCoordsPresent);
	}
	if (currentModel != nullptr) models.push_back(currentModel);

	return models;
}

ModelFragment* ResourceLoader::extractRawDataFromStorage(std::vector<face>& faces, std::vector<glm::vec3>& verticies, std::vector<glm::vec2>& texCoords, std::vector<glm::vec3>& normals, bool texCoordsPresent = true) {
	ModelFragment *modelFragment = new ModelFragment();
	
	modelFragment->vertices = new float[faces.size() * 3 * 4];
	modelFragment->texCoords = new float[faces.size() * 3 * 2];
	modelFragment->normals = new float[faces.size() * 3 * 4];
	modelFragment->colors = new float[0];

	modelFragment->vertexCount = faces.size() * 3;

	modelFragment->vertexSize = 4;
	modelFragment->texCoordSize = (texCoordsPresent) ? 2 : 0;
	modelFragment->normalSize = 4;
	modelFragment->colorSize = 0;

	for (int i = 0; i < faces.size(); i++) {
		for (int j = 0; j < 3; j++) {
			modelFragment->vertices[12 * i + 4 * j] = verticies[faces[i].vertices[j]].x;
			modelFragment->vertices[12 * i + 4 * j + 1] = verticies[faces[i].vertices[j]].y;
			modelFragment->vertices[12 * i + 4 * j + 2] = verticies[faces[i].vertices[j]].z;
			modelFragment->vertices[12 * i + 4 * j + 3] = 1.0f;

			if (texCoordsPresent) {
				modelFragment->texCoords[6 * i + 2 * j] = texCoords[faces[i].texCoords[j]].x;
				modelFragment->texCoords[6 * i + 2 * j + 1] = texCoords[faces[i].texCoords[j]].y;
			}

			modelFragment->normals[12 * i + 4 * j] = normals[faces[i].normals[j]].x;
			modelFragment->normals[12 * i + 4 * j + 1] = normals[faces[i].normals[j]].y;
			modelFragment->normals[12 * i + 4 * j + 2] = normals[faces[i].normals[j]].z;
			modelFragment->normals[12 * i + 4 * j + 3] = 0.0f;
		}
	}
	return modelFragment;
}