#include "ModelImporter.h"

Model* ModelImporter::importModel(const char* path, const char* modelName) {
	ifstream file(string(path) + "/" + string(modelName));
	
	if (file.is_open() == false) {
		throw "Miêso!";
	}

	Model* model = new Model(modelName);

	int fragments;
	string material, fragmentName;

	file >> fragments;

	for (int i = 0; i < fragments; i++) {
		file >> material >> fragmentName;
		model->fragments[material] = ModelFragmentImporter::importModelFragment((string(path) + "/" + string(fragmentName)).c_str());
	}

	file.close();

	return model;
}