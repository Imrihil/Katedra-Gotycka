#include "ModelFragmentExporter.h"

void ModelFragmentExporter::exportModelFragment(ModelFragment* modelFragment, const char* fileName) {
	FILE* file;
	fopen_s(&file, fileName, "wb");
	cout << fwrite(&(modelFragment->vertexCount), sizeof(int), 1, file) << endl;
	cout << fwrite(&(modelFragment->vertexSize), sizeof(int), 1, file) << endl;
	cout << fwrite(&(modelFragment->normalSize), sizeof(int), 1, file) << endl;
	cout << fwrite(&(modelFragment->texCoordSize), sizeof(int), 1, file) << endl;
	cout << fwrite(&(modelFragment->colorSize), sizeof(int), 1, file) << endl;
	cout << fwrite(modelFragment->vertices, sizeof(float)*modelFragment->vertexSize, modelFragment->vertexCount, file) << endl;
	cout << fwrite(modelFragment->normals, sizeof(float)*modelFragment->normalSize, modelFragment->vertexCount, file) << endl;
	cout << fwrite(modelFragment->texCoords, sizeof(float)*modelFragment->texCoordSize, modelFragment->vertexCount, file) << endl;
	cout << fwrite(modelFragment->colors, sizeof(float)*modelFragment->colorSize, modelFragment->vertexCount, file) << endl;
	fclose(file);
}