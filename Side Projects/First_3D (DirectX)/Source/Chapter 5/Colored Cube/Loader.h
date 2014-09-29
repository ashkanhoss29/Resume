#ifndef LOADER_H
#define LOADER_H

#include "..\..\Common\d3dUtil.h"
#include "Mesh.h"
#include <vector>

namespace loader
{
	std::vector<Mesh*> loadFbx(ID3D10Device* device, const char* filename);
	Mesh* load3ds(ID3D10Device* device, char* filename);
};

#endif