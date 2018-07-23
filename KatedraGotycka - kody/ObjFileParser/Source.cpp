#include <iostream>
#include <vector>
#include "ResourceLoader.h"
#include "Model.h"
#include "ModelFragment.h"
#include "ModelExporter.h"

using namespace std;

int main(int argc, char* argv[]) {
	cout << "Hello world!" << endl;

	string modelFile = "Katedra.obj";

	if (argc > 1) cout << (modelFile = argv[1]) << endl;

	ofstream file(modelFile + ".group");
	vector<Model*> models = ResourceLoader::loadModel(modelFile.c_str());

	file << models.size() << endl;

	for (auto model : models) {
		file << model->name << endl;
		ModelExporter::exportModel(model, (model->name).c_str());
	}

	file.close();

	system("PAUSE");

	return 0;
}