#include "ModelFragmentImporter.h"

ModelFragment* ModelFragmentImporter::importModelFragment(const char* fileName) {

	FILE* file;
	fopen_s(&file, fileName, "rb");

	ModelFragment *modelFragment = new ModelFragment();

	/*fseek(file, 0, SEEK_END);
	cout << "Total file size: " << ftell(file) << "\n";
	fseek(file, 0, SEEK_SET);*/

	fread(&(modelFragment->vertexCount), sizeof(int), 1, file);
	fread(&(modelFragment->vertexSize), sizeof(int), 1, file);
	fread(&(modelFragment->normalSize), sizeof(int), 1, file);
	fread(&(modelFragment->texCoordSize), sizeof(int), 1, file);
	fread(&(modelFragment->colorSize), sizeof(int), 1, file);

	/*cout << "Vertices: " << modelFragment->vertexCount << endl;
	cout << "Vertex size: " << modelFragment->vertexSize << endl;
	cout << "Normal size: " << modelFragment->normalSize << endl;
	cout << "TexCoord size: " << modelFragment->texCoordSize << endl;
	cout << "Color size: " << modelFragment->colorSize << endl;*/

	modelFragment->vertices = new float[modelFragment->vertexCount * modelFragment->vertexSize];
	modelFragment->normals = new float[modelFragment->vertexCount * modelFragment->normalSize];
	modelFragment->texCoords = new float[modelFragment->vertexCount * modelFragment->texCoordSize];
	modelFragment->colors = new float[modelFragment->vertexCount * modelFragment->colorSize];

	/*cout << "Read " << */
	fread(modelFragment->vertices, sizeof(float)*modelFragment->vertexSize, modelFragment->vertexCount, file);
	// << " vertices\n";
	fread(modelFragment->normals, sizeof(float)*modelFragment->normalSize, modelFragment->vertexCount, file);
	fread(modelFragment->texCoords, sizeof(float)*modelFragment->texCoordSize, modelFragment->vertexCount, file);
	fread(modelFragment->colors, sizeof(float)*modelFragment->colorSize, modelFragment->vertexCount, file);

	fclose(file);

	return modelFragment;
}