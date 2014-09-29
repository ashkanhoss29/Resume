#ifndef LOAD3DS_H
#define LOAD3DS_H

#include "..\..\Common\d3dUtil.h"
#include "Vertex.h"

namespace load3ds
{
	int load(ID3D10Device* device, ID3DX10Mesh* mMeshData, char* filename);
};

#endif