#include "ModelExporter.h"

void ModelExporter::exportModel(Model* model, const char* fileName) {

	if (CreateDirectory(fileName, NULL) != 0) {
		cout << GetLastError() << endl;
	}

	ofstream file;

	file.open((string(fileName) + "/" + string(fileName) + ".model").c_str());

	if (file.is_open() == false) {
		throw "Miêso!";
	}

	file << model->fragments.size() << endl;

	for (auto modelFragment : model->fragments) {
		file << modelFragment.first << " " << (model->name + "_" + modelFragment.first + ".frag") << endl;
		ModelFragmentExporter::exportModelFragment(modelFragment.second, (string(fileName) + "/" + model->name + "_" + modelFragment.first + ".frag").c_str());
	}

	file.close();
}
