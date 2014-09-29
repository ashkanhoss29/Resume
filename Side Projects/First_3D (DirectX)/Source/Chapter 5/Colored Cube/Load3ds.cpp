#include "Load3ds.h"
#include <stdio.h>
#include <io.h> //filelength()
#include <iostream>

using namespace std;

int load3ds::load(ID3D10Device* device, ID3DX10Mesh* mMeshData, char* filename)
{
	int i;
	FILE *l_file;
	unsigned short l_chunk_id;
	unsigned int l_chunk_length;
	unsigned char l_char;
	unsigned short l_qty;
	unsigned short l_face_flags;

	Vertex* mVertices;
	DWORD* mIndices;
	unsigned short* tempIndices;
	UINT* attributeBuffer;
	UINT mNumVertices;
	UINT mNumFaces;


	//Read .3ds file
	if ((l_file=fopen (filename, "rb"))== NULL) return -1; //Open the file
	while (ftell(l_file) < filelength(fileno(l_file))) //Loop to scan the whole file 
	{
		fread(&l_chunk_id, 2, 1, l_file); //Read the chunk header (2 bytes)
		fread(&l_chunk_length, 4, 1, l_file); //Read the length of the chunk (4 bytes)

		//Process data depending on the chunk
		switch (l_chunk_id)
		{

		case 0x4d4d: //Main chunk
			break;

		case 0x3d3d: //3D Editor chunk
			break;

		case 0x4000: //Object chunk (read name of object)
			i=0;
			do
			{
				fread(&l_char, 1, 1, l_file);
				//read name here later
				i++;
			} while (l_char != '\0' && i<20);
			break;

		case 0x4100: //Triangle Mesh chunk
			break;

		case 0x4110: //Vertices List chunk (read vertex positions)
			fread(&l_qty, sizeof(unsigned short), 1, l_file);
			mNumVertices = l_qty;
			mVertices = new Vertex[l_qty];
			cout << "********Vertices******** \n";
			for(i=0; i<l_qty; i++)
			{
				fread(&mVertices[i].pos.x, sizeof(float), 1, l_file); cout << mVertices[i].pos.x << " ";
				fread(&mVertices[i].pos.y, sizeof(float), 1, l_file); cout << mVertices[i].pos.y << " ";
				fread(&mVertices[i].pos.z, sizeof(float), 1, l_file); cout << mVertices[i].pos.z << "\n";

				mVertices[i].normal = D3DXVECTOR3(1.0f,0.0f,0.0f);
				mVertices[i].tangent = D3DXVECTOR3(1.0f,0.0f,0.0f);
				mVertices[i].texC = D3DXVECTOR2(1.0f,0.0f);
			}
			break;

		case 0x4120: //Face Description chunk (read indices)
			fread(&l_qty, sizeof(unsigned short), 1, l_file);
			mNumFaces = l_qty;
			mIndices = new DWORD[l_qty*3];
			tempIndices = new unsigned short[l_qty*3];
			attributeBuffer = new UINT[l_qty];
			cout << "\n *********Indices********* " << mNumFaces << "\n";
			for(i=0; i<l_qty; i++)
			{
				fread(&tempIndices[i*3+0], sizeof(unsigned short), 1, l_file);
				fread(&tempIndices[i*3+1], sizeof(unsigned short), 1, l_file);
				fread(&tempIndices[i*3+2], sizeof(unsigned short), 1, l_file);
				fread(&l_face_flags, sizeof(unsigned short), 1, l_file);
				attributeBuffer[i] = 0;

				mIndices[i*3+0] = tempIndices[i*3+0]; cout << mIndices[i*3+0] << " ";
				mIndices[i*3+1] = tempIndices[i*3+1]; cout << mIndices[i*3+1] << " ";
				mIndices[i*3+2] = tempIndices[i*3+2]; cout << mIndices[i*3+2] << "\n ";
			}
			break;

		case 0x4140: //Mapping Coordinates List chunk (read texture coordinates)
			fread(&l_qty, sizeof(unsigned short), 1, l_file);
			std::cout<<"\n\ntesting";
			for(i=0; i<l_qty; i++)
			{
				fread(&mVertices[i].texC.x, sizeof(float), 1, l_file); std::cout<<mVertices[i].texC.x<<" ";
				fread(&mVertices[i].texC.y, sizeof(float), 1, l_file); std::cout<<mVertices[i].texC.y<<"\n";
			}
			break;

		default:
			fseek(l_file, l_chunk_length-6, SEEK_CUR);
		}
	}

	fclose(l_file);


	//Calculate normals

	for(DWORD i = 0; i < mNumFaces; ++i)
	{
		// Get the indices for this triangle
		DWORD i0 = mIndices[i*3+0];
		DWORD i1 = mIndices[i*3+1];
		DWORD i2 = mIndices[i*3+2];

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
	cout << "********Normals********\n";
	// Normalize the added normals for each vertex
	for(DWORD j = 0; j < mNumVertices; ++j)
	{
		D3DXVec3Normalize(&mVertices[j].normal, &mVertices[j].normal);
		//cout << mVertices[j].normal.x << " " << mVertices[j].normal.y << " " << mVertices[j].normal.z << "\n";
	}


	cout << "\nmNumVertices " << mNumVertices << "\n";
	cout << "mNumberFaces " << mNumFaces;


	//Load data read from .3ds file into mMeshData
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};
	HR(D3DX10CreateMesh(device, vertexDesc, 4, 
		vertexDesc[0].SemanticName, mNumVertices, 
		mNumFaces, D3DX10_MESH_32_BIT, &mMeshData));
	
	HR(mMeshData->SetVertexData(0, mVertices));
	HR(mMeshData->SetIndexData(mIndices, mNumFaces*3));
	HR(mMeshData->SetAttributeData(attributeBuffer));

	delete[] mVertices;
	delete[] mIndices;
	delete[] attributeBuffer;

	HR(mMeshData->GenerateAdjacencyAndPointReps(0.001f));
	HR(mMeshData->Optimize(D3DX10_MESHOPT_ATTR_SORT|D3DX10_MESHOPT_VERTEX_CACHE,0,0));
	HR(mMeshData->CommitToDevice());

	return mNumFaces;
}