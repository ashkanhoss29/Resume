#ifndef PLANE_H
#define PLANE_H

#include "..\..\Common\d3dUtil.h"

class Plane
{
public:

	Plane();
	~Plane();

	void init(ID3D10Device* device, float scale);
	void draw();

private:
	DWORD mNumVertices;
	DWORD mNumFaces;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVertexBuffers[2]; //vertex buffer
	ID3D10Buffer* mIndexBuffer; //index buffer
};

#endif