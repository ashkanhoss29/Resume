#ifndef MESH_H
#define MESH_H

#include "..\..\Common\d3dUtil.h"
#include <string>

struct Mesh
{
	Mesh()
		: fileType(""),
		  name(""),
		  mMeshData(0),
		  mDiffuseTexture(0),
		  mSpecularTexture(0),
		  mNormalTexture(0)
	{
	}

	~Mesh()
	{
		ReleaseCOM(mMeshData);
		ReleaseCOM(mDiffuseTexture);
		ReleaseCOM(mAlphaTexture);
		ReleaseCOM(mSpecularTexture);
		ReleaseCOM(mNormalTexture);
	}

	std::string fileType;
	std::string name;
	ID3DX10Mesh* mMeshData;
	ID3D10ShaderResourceView* mDiffuseTexture;
	ID3D10ShaderResourceView* mAlphaTexture;
	ID3D10ShaderResourceView* mSpecularTexture;
	ID3D10ShaderResourceView* mNormalTexture;
};

#endif