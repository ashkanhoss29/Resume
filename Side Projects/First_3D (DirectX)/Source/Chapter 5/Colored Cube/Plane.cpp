#include "Plane.h"
#include "Vertex.h"

Plane::Plane()
: mNumVertices(0), mNumFaces(0), md3dDevice(0), mIndexBuffer(0)
{
	mVertexBuffers[0] = NULL;
	mVertexBuffers[1] = NULL;
}

Plane::~Plane()
{
	ReleaseCOM(mVertexBuffers[0]);
	ReleaseCOM(mVertexBuffers[1]);
	ReleaseCOM(mIndexBuffer);
}

void Plane::init(ID3D10Device* device, float scale)
{
	md3dDevice = device; //the direct3d device pointer is pass!

	mNumVertices = 100; //number of vertices...duh
	mNumFaces = 50; //number of triangles?


	/*VERTEX BUFFER CREATION*/
	//Create the vertices that will be used to create the vertex buffer
	Vertex* mVertices = new Vertex[mNumVertices];

	D3DXVECTOR3* mPosVertices = new D3DXVECTOR3[mNumVertices];
	D3DXVECTOR2* mTextureVertices = new D3DXVECTOR2[mNumVertices];
	DWORD* indices = new DWORD[mNumFaces*3];

	int faceIndex = 0;
	for(int i = 0; i < 5; ++i)
	{
		for(int j = 0; j < 5; ++j)
		{
			float x = i*10.0f-25.0f;
			float y = j*10.0f-25.0f;

			mPosVertices[faceIndex*4+0] = D3DXVECTOR3( 0.0f+x,  -2.0f,  0.0f+y  );
			mPosVertices[faceIndex*4+1] = D3DXVECTOR3( 0.0f+x,  -2.0f,  10.0f+y );
			mPosVertices[faceIndex*4+2] = D3DXVECTOR3( 10.0f+x, -2.0f,  10.0f+y );
			mPosVertices[faceIndex*4+3] = D3DXVECTOR3( 10.0f+x, -2.0f,  0.0f+y  );

			mTextureVertices[faceIndex*4+0] = D3DXVECTOR2(0.0f+j, 1.0f+i);
			mTextureVertices[faceIndex*4+1] = D3DXVECTOR2(0.0f+j, 0.0f+i);
			mTextureVertices[faceIndex*4+2] = D3DXVECTOR2(1.0f+j, 0.0f+i);
			mTextureVertices[faceIndex*4+3] = D3DXVECTOR2(1.0f+j, 1.0f+i);

			indices[faceIndex*6+0] = 0 + faceIndex*4;
			indices[faceIndex*6+1] = 1 + faceIndex*4;
			indices[faceIndex*6+2] = 2 + faceIndex*4;
			indices[faceIndex*6+3] = 0 + faceIndex*4;
			indices[faceIndex*6+4] = 2 + faceIndex*4;
			indices[faceIndex*6+5] = 3 + faceIndex*4;

			faceIndex++;
		}
	}

	/*
	D3DXVECTOR3 mPosVertices[] =
	{
		D3DXVECTOR3(  0.0f, -2.0f,   0.0f),
		D3DXVECTOR3(  0.0f, -2.0f,  50.0f),
		D3DXVECTOR3(+50.0f, -2.0f, +50.0f),
		D3DXVECTOR3( 50.0f, -2.0f,   0.0f),

		D3DXVECTOR3(-50.0f, -2.0f,   0.0f),
		D3DXVECTOR3(-50.0f, -2.0f, +50.0f),
		D3DXVECTOR3(  0.0f, -2.0f, +50.0f),
		D3DXVECTOR3(  0.0f, -2.0f,   0.0f),

		D3DXVECTOR3(-50.0f, -2.0f, -50.0f),
		D3DXVECTOR3(-50.0f, -2.0f,   0.0f),
		D3DXVECTOR3(  0.0f, -2.0f,   0.0f),
		D3DXVECTOR3(  0.0f, -2.0f, -50.0f),

		D3DXVECTOR3(  0.0f, -2.0f, -50.0f),
		D3DXVECTOR3(  0.0f, -2.0f,   0.0f),
		D3DXVECTOR3(+50.0f, -2.0f,   0.0f),
		D3DXVECTOR3(+50.0f, -2.0f, -50.0f),
	};

	D3DXVECTOR2 mTextureVertices[] =
	{
		/*
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(1.0f, 1.0f)
		/

		D3DXVECTOR2(1.0f, 1.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(2.0f, 0.0f),
		D3DXVECTOR2(2.0f, 1.0f),

		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(1.0f, 1.0f),

		D3DXVECTOR2(0.0f, 2.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f),
		D3DXVECTOR2(1.0f, 2.0f),

		D3DXVECTOR2(1.0f, 2.0f),
		D3DXVECTOR2(1.0f, 1.0f),
		D3DXVECTOR2(2.0f, 1.0f),
		D3DXVECTOR2(2.0f, 2.0f),
	};
	

	//INDEX BUFFER CREATION
	//We've made the vertex buffer, time to make the index buffer...bro
	DWORD indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		//2, 1, 0,
		//3, 2, 0,

		4, 5, 6,
		4, 6, 7,
		//6, 5, 4,
		//7, 6, 4,

		8, 9, 10,
		8, 10, 11,
		//10, 9, 8,
		//11, 10, 8,

		12, 13, 14,
		12, 14, 15,
		//14, 13, 12,
		//15, 14, 12
	};
	*/

	for(int v = 0; v < mNumVertices; v++)
	{
		mVertices[v].pos = mPosVertices[v];
		mVertices[v].texC = mTextureVertices[v];
	}


	//Init normals to <0,0,0>
	for(DWORD i = 0; i < mNumVertices; ++i)
	{
		mVertices[i].normal = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}
	
	// Find normals
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
		mVertices[i].tangent = D3DXVECTOR3(1.0f,0.0f,0.0f);
	}
	

	//Scale the plane! What is scale? Hell I don't know!
	for(DWORD i = 0; i < mNumVertices; ++i)
		mVertices[i].pos *= scale;


	//Ok dude here we're about to actually create the vertex buffer
	D3D10_BUFFER_DESC pvbd;
	pvbd.Usage = D3D10_USAGE_IMMUTABLE;
	pvbd.ByteWidth = sizeof(Vertex) * mNumVertices;
	pvbd.BindFlags = D3D10_BIND_VERTEX_BUFFER; //this says we're describing a vertex buffer
	pvbd.CPUAccessFlags = 0;
	pvbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA posVertexInitData; //stores our array so we can pass it to CreateBuffer
	posVertexInitData.pSysMem = mVertices; //see, I told you ^
	HR(md3dDevice->CreateBuffer(&pvbd, &posVertexInitData, &mVertexBuffers[0])); //YAY WE MADE THE BUFFER! It's stored in mVB

	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIndexBuffer));

	delete mVertices;
	delete mPosVertices;
	delete mTextureVertices;
	delete indices;
}

void Plane::draw()
{
	UINT stride[1] = 
	{
		sizeof(Vertex),
	};
	UINT offset[2] = {0,0};
	md3dDevice->IASetVertexBuffers(0, 1, mVertexBuffers, stride, offset);
	md3dDevice->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0); //here we set the format to R32_UINT because our indices are DWORD
	md3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
}