//=======================================================================================
// Box.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "Box.h"
#include "Vertex.h"
#include <fstream>

Box::Box()
: mNumVertices(0), mNumFaces(0), md3dDevice(0), mIndexBuffer(0)
{
	mVertexBuffers[0] = NULL;
}
 
Box::~Box()
{
	ReleaseCOM(mVertexBuffers[0]);
	ReleaseCOM(mIndexBuffer);
}

void Box::init(ID3D10Device* device, float scale)
{
	md3dDevice = device;
 
	mNumVertices = 24;
	mNumFaces    = 12; // 2 per quad

	// Create vertex buffer
	Vertex* mVertices = new Vertex[24];
	
	D3DXVECTOR3 mPosVertices[] =
    {
		D3DXVECTOR3(-1.0f, -1.0f, -1.0f),
		D3DXVECTOR3(-1.0f, +1.0f, -1.0f),
		D3DXVECTOR3(+1.0f, +1.0f, -1.0f),
		D3DXVECTOR3(+1.0f, -1.0f, -1.0f),

		D3DXVECTOR3(-1.0f, -1.0f, +1.0f),
		D3DXVECTOR3(+1.0f, -1.0f, +1.0f),
		D3DXVECTOR3(+1.0f, +1.0f, +1.0f),
		D3DXVECTOR3(-1.0f, +1.0f, +1.0f),

		D3DXVECTOR3(-1.0f, +1.0f, -1.0f),
		D3DXVECTOR3(-1.0f, +1.0f, +1.0f),
		D3DXVECTOR3(+1.0f, +1.0f, +1.0f),
		D3DXVECTOR3(+1.0f, +1.0f, -1.0f),

		D3DXVECTOR3(-1.0f, -1.0f, -1.0f),
		D3DXVECTOR3(+1.0f, -1.0f, -1.0f),
		D3DXVECTOR3(+1.0f, -1.0f, +1.0f),
		D3DXVECTOR3(-1.0f, -1.0f, +1.0f),

		D3DXVECTOR3(-1.0f, -1.0f, +1.0f),
		D3DXVECTOR3(-1.0f, +1.0f, +1.0f),
		D3DXVECTOR3(-1.0f, +1.0f, -1.0f),
		D3DXVECTOR3(-1.0f, -1.0f, -1.0f),

		D3DXVECTOR3(+1.0f, -1.0f, -1.0f),
		D3DXVECTOR3(+1.0f, +1.0f, -1.0f),
		D3DXVECTOR3(+1.0f, +1.0f, +1.0f),
		D3DXVECTOR3(+1.0f, -1.0f, +1.0f),
    };

	D3DXVECTOR2 mTextureVertices[] =
	{
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(1.0f, 1.0f),

		D3DXVECTOR2(1.0f, 1.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),

		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(1.0f, 1.0f),

		D3DXVECTOR2(1.0f, 1.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),

		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(1.0f, 1.0f),

		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(1.0f, 1.0f),
	};

	// Create the index buffer
	DWORD indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 5, 6,
		4, 6, 7,

		// left face
		8, 9, 10,
		8, 10, 11,

		// right face
		12, 13, 14,
		12, 14, 15,

		// top face
		16, 17, 18,
		16, 18, 19,

		// bottom face
		20, 21, 22, 
		20, 22, 23
	};

	for(int v = 0; v < mNumVertices; v++)
	{
		mVertices[v].pos = mPosVertices[v];
		mVertices[v].texC = mTextureVertices[v];

		/* This uses calculas to find nomrals, we don't want this
		D3DXVECTOR3 normal;
		normal.x = -0.03f * mPosVertices[v].z * cosf(0.1f * mPosVertices[v].x) - 0.3f * cosf(0.1f * mPosVertices[v].z);
		normal.y = 1.0f;
		normal.z = -0.3f * sinf(0.1f * mPosVertices[v].x) + 0.03f * mPosVertices[v].x * sinf(0.1f * mPosVertices[v].z);
		D3DXVec3Normalize(&mVertices[v].normal, &normal);
		*/
	}

	//Set normals to 0
	for(DWORD i = 0; i < mNumVertices; ++i)
	{
		mVertices[i].normal = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}

	//Calculate normals
	for(DWORD i = 0; i < mNumFaces; ++i)
	{
		// Get the indices for this triangle
		DWORD i0 = indices[i*3+0];
		DWORD i1 = indices[i*3+1];
		DWORD i2 = indices[i*3+2];

		// Get the vertices for this triangle
		Vertex v0 = mVertices[i0];
		Vertex v1 = mVertices[i1];
		Vertex v2 = mVertices[i2];

		// Compute the normal of the triangle by taking the cross of two sides of the triangle
		D3DXVECTOR3 e0 = v1.pos - v0.pos;
		D3DXVECTOR3 e1 = v2.pos - v0.pos;

		D3DXVECTOR3 faceNormal;
		D3DXVec3Cross(&faceNormal, &e0, &e1);

		// Add the computed normal to each vertex of the triangle
		mVertices[i0].normal += faceNormal;
		mVertices[i1].normal += faceNormal;
		mVertices[i2].normal += faceNormal;
	}

	// Normalize the added normals for each vertex
	for(DWORD i = 0; i < mNumVertices; ++i)
	{
		D3DXVec3Normalize(&mVertices[i].normal, &mVertices[i].normal);
	}

	mVertices[0].tangent  = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	mVertices[1].tangent  = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	mVertices[2].tangent  = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	mVertices[3].tangent  = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	mVertices[4].tangent  = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	mVertices[5].tangent  = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	mVertices[6].tangent  = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	mVertices[7].tangent  = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	mVertices[8].tangent  = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	mVertices[9].tangent  = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	mVertices[10].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	mVertices[11].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	mVertices[12].tangent = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	mVertices[13].tangent = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	mVertices[14].tangent = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	mVertices[15].tangent = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	mVertices[16].tangent = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	mVertices[17].tangent = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	mVertices[18].tangent = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	mVertices[19].tangent = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	mVertices[20].tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	mVertices[21].tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	mVertices[22].tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	mVertices[23].tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);


	// Scale the box.
	for(DWORD i = 0; i < mNumVertices; ++i)
		mVertices[i].pos *= scale;


    D3D10_BUFFER_DESC posvbd;
    posvbd.Usage = D3D10_USAGE_IMMUTABLE;
    posvbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    posvbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    posvbd.CPUAccessFlags = 0;
    posvbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vertexInitData;
    vertexInitData.pSysMem = mVertices;
	HR(md3dDevice->CreateBuffer(&posvbd, &vertexInitData, &mVertexBuffers[0]));


	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIndexBuffer));
}

void Box::update(float dt)
{
}

void Box::draw()
{
	UINT stride[1] =
	{
		sizeof(Vertex),
	};
	UINT offset[1] = {0};
	md3dDevice->IASetVertexBuffers(0, 1, mVertexBuffers, stride, offset);
	md3dDevice->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	md3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
}