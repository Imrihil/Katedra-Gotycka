#include "ModelFragment.h"


ModelFragment::ModelFragment()
{
}


ModelFragment::~ModelFragment()
{
	delete[] vertices;
	delete[] colors;
	delete[] normals;
	delete[] texCoords;
}
