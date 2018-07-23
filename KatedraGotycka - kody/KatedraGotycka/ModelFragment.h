#pragma once
class ModelFragment
{
public:
	float* vertices;
	float* colors;
	float* normals;
	float* texCoords;
	unsigned int vertexCount;
	unsigned int vertexSize;
	unsigned int colorSize;
	unsigned int normalSize;
	unsigned int texCoordSize;
	ModelFragment();
	virtual ~ModelFragment();
};

