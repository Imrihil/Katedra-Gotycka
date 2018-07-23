#include "MaterialImporter.h"

#include "GL\glew.h"
#include "tga.h"

GLuint readTexture(char* filename) {
	GLuint tex;
	TGAImg img;
	glActiveTexture(GL_TEXTURE0);
	if (img.Load(filename) == IMG_OK) {
		glGenTextures(1, &tex); //Zainicjuj uchwyt tex
		glBindTexture(GL_TEXTURE_2D, tex); //Przetwarzaj uchwyt tex
		if (img.GetBPP() == 24) //Obrazek 24bit
			glTexImage2D(GL_TEXTURE_2D, 0, 3, img.GetWidth(), img.GetHeight(), 0,
			GL_RGB, GL_UNSIGNED_BYTE, img.GetImg());
		else if (img.GetBPP() == 32) //Obrazek 32bit
			glTexImage2D(GL_TEXTURE_2D, 0, 4, img.GetWidth(), img.GetHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, img.GetImg());
		else {
			printf("Nieobs³ugiwany format obrazka w pliku: %s \n", filename);
		}
	}
	else {
		printf("B³¹d przy wczytywaniu pliku: %s\n", filename);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return tex;
}

map<string, Material> MaterialImporter::loadMtlFile(const char* filename) {
	ifstream in(filename, ios::in);
	map<string, Material> materials;

	if (!in) throw NULL;

	string line;

	while (getline(in, line)) {
		if (line.substr(0, 6) == "newmtl") {
			materials[line.substr(7)] = readMaterial(&in);
		}
	}

	return materials;
}


Material MaterialImporter::readMaterial(ifstream *in) {
	string line;
	Material material;
	do {
		getline(*in, line);
		if (line.substr(0, 2) == "Ka") {
			istringstream s(line.substr(2));
			s >> material.Ka.x >> material.Ka.y >> material.Ka.z;
		}
		else if (line.substr(0, 2) == "Kd") {
			istringstream s(line.substr(2));
			glm::vec3 v;
			s >> material.Kd.x >> material.Kd.y >> material.Kd.z;
		}
		else if (line.substr(0, 2) == "Ks") {
			istringstream s(line.substr(2));
			glm::vec3 v;
			s >> material.Ks.x >> material.Ks.y >> material.Ks.z;
		}
		else if (line.substr(0, 2) == "Ns") {
			istringstream s(line.substr(2));
			s >> material.Ns;
		}
		else if (line.substr(0, 2) == "Ni") {
			istringstream s(line.substr(2));
			s >> material.Ni;
		}
		else if (line.substr(0, 1) == "d") {
			istringstream s(line.substr(1));
			s >> material.d;
		}
		else if (line.substr(0, 5) == "illum") {
			istringstream s(line.substr(5));
			s >> material.illum;
		}
		else if (line.substr(0, 6) == "map_Kd") {
			istringstream s(line.substr(6));
			s >> material.map_Kd;
		}
		else if (line.substr(0, 5) == "map_d") {
			istringstream s(line.substr(5));
			s >> material.map_d;
		}
		else if (line.substr(0, 6) == "map_Ks") {
			istringstream s(line.substr(6));
			s >> material.map_Ks;
		}
		else {
			//cout << line << "+\n";
		}
	} while (line != "");
	if (material.map_Kd != "") {
		//cout << "READING TEXTURE FOR " << material.map_Kd << endl;
		material.diffuseTextureHandle = readTexture(&(material.map_Kd[0]));
	}
	if (material.map_Ks != "") {
		material.specularTextureHandle = readTexture(&(material.map_Ks[0]));
	}
	return material;
}