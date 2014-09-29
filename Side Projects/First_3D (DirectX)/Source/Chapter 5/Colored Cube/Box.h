//=======================================================================================
// Box.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef BOX_H
#define BOX_H

#include "..\..\Common\d3dUtil.h"

class Box
{
public:

	Box();
	~Box();

	void init(ID3D10Device* device, float scale);
	void update(float dt);
	void draw();

private:
	DWORD mNumVertices;
	DWORD mNumFaces;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVertexBuffers[1];
	ID3D10Buffer* mIndexBuffer;
};


#endif // BOX_H
