#include "Loader.h"
#include "Vertex.h"
#include <fbxsdk.h>
#include <istream>
#include <fstream>
#include <io.h>
#include <iostream>

using namespace std;

ID3D10Device* mDevice = NULL; //consider not making global!!!!

std::vector<Mesh*> ProcessNode(FbxNode* pNode);
Mesh* ProcessAttribute(FbxNodeAttribute* pAttribute);
Mesh* ParseMesh(FbxNode* pNode);
KFbxXMatrix ComputeTotalMatrix(KFbxNode* pNode);
void TriangulateMesh(FbxNode* pNode);
void DisplayTexture(FbxGeometry* pGeometry);
void FindAndDisplayTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex);
void DisplayTextureInfo(FbxTexture* pTexture, int pBlendMode);


std::vector<Mesh*> loader::loadFbx(ID3D10Device* device, const char* filename)
{
	mDevice = device;

	vector<Mesh*> meshs;

	FbxManager* lSdkManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Open file
	if(!lImporter->Initialize(filename, -1, lSdkManager->GetIOSettings()))
	{
		return meshs;
	}

	// Create our scene to be populated
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	// Import contents of the file into our scene
	lImporter->Import(lScene);

	// We're done with importer
	lImporter->Destroy();

	// Step through all the nodes
	FbxNode* lRootNode = lScene->GetRootNode();
	if(lRootNode)
	{
		for(int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			std::vector<Mesh*> tempMeshs;
			tempMeshs = ProcessNode(lRootNode->GetChild(i));
			for(int j = 0; j < tempMeshs.size(); ++j)
			{
				meshs.push_back(tempMeshs.at(j));
			}
		}
	}

	return meshs;
}


std::vector<Mesh*> ProcessNode(FbxNode* pNode)
{
	std::vector<Mesh*> meshs;
	Mesh* mesh = NULL;

	const char* nodeName = pNode->GetName();
	fbxDouble3 translation = pNode->LclTranslation.Get();
	fbxDouble3 rotation = pNode->LclRotation.Get();
	fbxDouble3 scaling = pNode->LclScaling.Get();

	// step through attributes
	for(int i = 0; i < pNode->GetNodeAttributeCount(); i++)
	{
		if((mesh = ProcessAttribute(pNode->GetNodeAttributeByIndex(i))) != NULL)
		{
			meshs.push_back(mesh);
			mesh = NULL;
		}
	}

	for(int i = 0; i < pNode->GetChildCount(); i++)
	{
		std::vector<Mesh*> tempMesh = ProcessNode(pNode->GetChild(i));
		for(int j = 0; j < tempMesh.size(); ++j)
		{
			meshs.push_back(tempMesh.at(j));
		}
	}

	return meshs;
}


Mesh* ProcessAttribute(FbxNodeAttribute* pAttribute)
{
	if(!pAttribute) return NULL;

	FbxNodeAttribute::EType type = pAttribute->GetAttributeType();

	switch(type)
	{
	case FbxNodeAttribute::eUnknown:  return NULL; break;
	case FbxNodeAttribute::eNull:     return NULL; break;
	case FbxNodeAttribute::eMarker:   return NULL; break;
	case FbxNodeAttribute::eSkeleton: return NULL; break;
	case FbxNodeAttribute::eMesh:
		return ParseMesh( pAttribute->GetNode() );
		break;
	default: std::cout << "unkown ";
	}
	
	//FbxString name = pAttribute->GetName();
	//std::cout << name.Buffer() << "\n";

	return NULL;
}


Mesh* ParseMesh( FbxNode* pNode )
{
	TriangulateMesh( pNode );

	FbxMesh* pMesh = (FbxMesh*)pNode->GetNodeAttribute();

	int lPolygonCount = pMesh->GetPolygonCount();
	int lControlPointCount = pMesh->GetControlPointsCount();

	if(lPolygonCount == 0 || lControlPointCount == 0)
	{
		return NULL;
	}

	KFbxXMatrix TotalMatrix = ComputeTotalMatrix(pNode);
	FbxVector4* pVertexPositions = pMesh->GetControlPoints();
	std::vector<Vertex> newVertices;
	std::vector<DWORD> indices;

	// Compute normals if they don't exist
	FbxLayerElementArrayTemplate<FbxVector4>* pNormals = NULL;
	pMesh->GetNormals( &pNormals );
	if( pNormals == NULL )
	{
		pMesh->InitNormals();
		pMesh->ComputeVertexNormals();
	}

	// Step through polygons
	int vertexId = 0;
	for( int i = 0; i < lPolygonCount; ++i )
	{
		int polygonSize = pMesh->GetPolygonSize( i );

		// Step through control point vertices (indices)
		for( int j = 0; j < polygonSize; j++ )
		{
			Vertex tempVertex;

			int lControlPointIndex = pMesh->GetPolygonVertex( i, j );

			// ===== INDICES =====
			indices.push_back(vertexId);

			// ===== POSITIONS =====
			KFbxVector4 FbxPosition = pVertexPositions[lControlPointIndex];
			KFbxVector4 FinalPosition = TotalMatrix.MultT(FbxPosition);
			tempVertex.pos.x = FinalPosition.mData[0];
			tempVertex.pos.y = FinalPosition.mData[1];
			tempVertex.pos.z = FinalPosition.mData[2];


			// ===== NORMALS =====
			FbxVector4 normal;
			int normalCount = 0;
			for( int l = 0; l < pMesh->GetElementNormalCount(); ++l )
			{
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal( l );
				switch (leNormal->GetMappingMode())
				{
				case FbxGeometryElement::eByPolygonVertex:
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							normal = leNormal->GetDirectArray().GetAt(vertexId);
						}
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = leNormal->GetIndexArray().GetAt(vertexId);
							normal = leNormal->GetDirectArray().GetAt(id);
						}
					}
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							normal = leNormal->GetDirectArray().GetAt(lControlPointIndex);
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = leNormal->GetIndexArray().GetAt(lControlPointIndex);
							normal = leNormal->GetDirectArray().GetAt(lControlPointIndex);

						}
						break;
					}
					break;
				}

				tempVertex.normal.x = normal.mData[0];
				tempVertex.normal.y = normal.mData[1];
				tempVertex.normal.z = normal.mData[2];

				normalCount++;
			}
			if(normalCount > 1) std::cout << " WARNING: Unsupported normal count " << normalCount << "\n";


			// ===== TANGENTS =====
			FbxVector4 tangent;
			int tangentCount = 0;
			for( int l = 0; l < pMesh->GetElementTangentCount(); ++l )
			{
				FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent( l );
				switch (leTangent->GetMappingMode())
				{
				case FbxGeometryElement::eByPolygonVertex:
					switch (leTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							tangent = leTangent->GetDirectArray().GetAt(vertexId);
						}
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = leTangent->GetIndexArray().GetAt(vertexId);
							tangent = leTangent->GetDirectArray().GetAt(id);
						}
					}
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							tangent = leTangent->GetDirectArray().GetAt(lControlPointIndex);
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = leTangent->GetIndexArray().GetAt(lControlPointIndex);
							tangent = leTangent->GetDirectArray().GetAt(lControlPointIndex);

						}
						break;
					}
					break;
				}

				tempVertex.tangent.x = tangent.mData[0];
				tempVertex.tangent.y = tangent.mData[1];
				tempVertex.tangent.z = tangent.mData[2];

				tangentCount++;
			}
			//if(tangentCount != 1) std::cout << " WARNING: Unsupported tangent count " << tangentCount << "\n";


			// ===== UVs =====
			FbxVector2 texC;
			int UVCount = 0;
			for( int l = 0; l < pMesh->GetElementUVCount(); ++l )
			{
				FbxGeometryElementUV* leUV = pMesh->GetElementUV( l);
				switch (leUV->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
					{
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							{
								texC = leUV->GetDirectArray().GetAt(lControlPointIndex);
							}
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
								texC = leUV->GetDirectArray().GetAt(id);
							}
							break;
						default:
							break;// other reference modes not shown here!
						}
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
					{
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							{
								texC = leUV->GetDirectArray().GetAt(vertexId);
							}
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leUV->GetIndexArray().GetAt(vertexId);
								texC = leUV->GetDirectArray().GetAt(id);
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}

				tempVertex.texC.x = texC.mData[0];
				tempVertex.texC.y = 1.0f-texC.mData[1];

				UVCount++;
			}
			//if(UVCount != 1) std::cout<< " WARNING: Unsupported UV count " << UVCount << "\n";

			newVertices.push_back(tempVertex);

			vertexId++;
		}
	}
	std::cout << "vertexId " << vertexId << "\n";


	DisplayTexture(pMesh);


	// Load data read from file into Mesh
	Mesh* mMesh = new Mesh();
	ID3DX10Mesh* mMeshData = NULL;

	// Create vertex buffer
	Vertex* vertices = new Vertex[newVertices.size()];
	for(int i = 0; i < newVertices.size(); i++)
	{
		vertices[i] = newVertices.at(i);
	}

	// Create index buffer
	DWORD* mIndices = new DWORD[indices.size()];
	for(int i = 0; i < indices.size(); ++i)
	{
		mIndices[i] = indices.at(i);
	}

	// Create attribute buffer
	UINT* attributeBuffer = new UINT[lPolygonCount];
	for(int i = 0; i < lPolygonCount; ++i)
	{
		attributeBuffer[i] = 0;
	}

	// Flip triangle order to flip from RH to LH
	for(int i = 0; i < lPolygonCount; i++)
	{
		DWORD i1 = mIndices[i*3+1];
		mIndices[i*3+1] = mIndices[i*3+2];
		mIndices[i*3+2] = i1;
	}


	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};
	HR(D3DX10CreateMesh(mDevice, vertexDesc, 4, 
		vertexDesc[0].SemanticName, newVertices.size(), 
		lPolygonCount, D3DX10_MESH_32_BIT, &mMeshData));
	
	HR(mMeshData->SetVertexData(0, vertices));
	HR(mMeshData->SetIndexData(mIndices, lPolygonCount*3));
	HR(mMeshData->SetAttributeData(attributeBuffer));

	delete[] vertices;
	delete[] mIndices;
	delete[] attributeBuffer;

	HR(mMeshData->GenerateAdjacencyAndPointReps(0.001f));
	HR(mMeshData->Optimize(D3DX10_MESHOPT_ATTR_SORT|D3DX10_MESHOPT_VERTEX_CACHE,0,0));
	HR(mMeshData->CommitToDevice());

	mMesh->fileType = "fbx";
	mMesh->name = pNode->GetName();
	mMesh->mMeshData = mMeshData;

	return mMesh;

}


KFbxXMatrix ComputeTotalMatrix(KFbxNode* pNode)
{
	KFbxXMatrix Geometry;
	KFbxVector4 Translation, Rotation, Scaling;
	Translation = pNode->GetGeometricTranslation(KFbxNode::eSourcePivot);
	Rotation = pNode->GetGeometricRotation(KFbxNode::eSourcePivot);
	Scaling = pNode->GetGeometricScaling(KFbxNode::eSourcePivot);
	Geometry.SetT(Translation);
	Geometry.SetR(Rotation);
	Geometry.SetS(Scaling);

	FbxScene* lScene = pNode->GetScene();

	KFbxXMatrix& GlobalTransform = lScene->GetEvaluator()->GetNodeGlobalTransform(pNode);

	KFbxXMatrix axisChanger;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			axisChanger[i][j] = 0;
		}
	}

	// Change RH to LH
	axisChanger.mData[0][0] = 1;
	axisChanger.mData[1][1] = 1;
	axisChanger.mData[2][2] = -1;
	axisChanger.mData[3][3] = 1;
	GlobalTransform *= axisChanger;
 
	KFbxXMatrix TotalMatrix;
	TotalMatrix = GlobalTransform * Geometry;

	return TotalMatrix;
}


void TriangulateMesh(FbxNode* pNode)
{
	FbxMesh* pMesh = (FbxMesh*) pNode->GetNodeAttribute();

	if( pMesh->IsTriangleMesh() )
	{
		std::cout << "already triangulated!!! \n";
		return;
	}

	FbxGeometryConverter lConvertor(pNode->GetFbxManager());
	bool status = lConvertor.TriangulateInPlace( pNode );

	if( !status )
	{
		std::cout << "failed to triangulate mesh\n";
	}
}



void DisplayTexture(FbxGeometry* pGeometry)
{
    int lMaterialIndex;
    FbxProperty lProperty;
    if(pGeometry->GetNode()==NULL)
        return;
    int lNbMat = pGeometry->GetNode()->GetSrcObjectCount(FbxSurfaceMaterial::ClassId);
    for (lMaterialIndex = 0; lMaterialIndex < lNbMat; lMaterialIndex++){
        FbxSurfaceMaterial *lMaterial = (FbxSurfaceMaterial *)pGeometry->GetNode()->GetSrcObject(FbxSurfaceMaterial::ClassId, lMaterialIndex);
        bool lDisplayHeader = true;

        //go through all the possible textures
        if(lMaterial){

            int lTextureIndex;
            FBXSDK_FOR_EACH_TEXTURE(lTextureIndex)
            {
                lProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[lTextureIndex]);
                FindAndDisplayTextureInfoByProperty(lProperty, lDisplayHeader, lMaterialIndex); 
            }

        }//end if(lMaterial)

    }// end for lMaterialIndex     
}


void FindAndDisplayTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex)
{
    if( pProperty.IsValid() )
    {
        int lTextureCount = pProperty.GetSrcObjectCount(FbxTexture::ClassId);

        for (int j = 0; j < lTextureCount; ++j)
        {
            //Here we have to check if it's layeredtextures, or just textures:
            FbxLayeredTexture *lLayeredTexture = FbxCast <FbxLayeredTexture>(pProperty.GetSrcObject(FbxLayeredTexture::ClassId, j));
            if (lLayeredTexture)
            {
                //DisplayInt("    Layered Texture: ", j);
                FbxLayeredTexture *lLayeredTexture = FbxCast <FbxLayeredTexture>(pProperty.GetSrcObject(FbxLayeredTexture::ClassId, j));
                int lNbTextures = lLayeredTexture->GetSrcObjectCount(FbxTexture::ClassId);
                for(int k =0; k<lNbTextures; ++k)
                {
                    FbxTexture* lTexture = FbxCast <FbxTexture> (lLayeredTexture->GetSrcObject(FbxTexture::ClassId,k));
                    if(lTexture)
                    {

                        if(pDisplayHeader){                    
                            //DisplayInt("    Textures connected to Material ", pMaterialIndex);
                            pDisplayHeader = false;
                        }

                        //NOTE the blend mode is ALWAYS on the LayeredTexture and NOT the one on the texture.
                        //Why is that?  because one texture can be shared on different layered textures and might
                        //have different blend modes.

                        FbxLayeredTexture::EBlendMode lBlendMode;
                        lLayeredTexture->GetTextureBlendMode(k, lBlendMode); 
                        DisplayTextureInfo(lTexture, (int) lBlendMode);   
                    }

                }
            }
            else
            {
                //no layered texture simply get on the property
                FbxTexture* lTexture = FbxCast <FbxTexture> (pProperty.GetSrcObject(FbxTexture::ClassId,j));
                if(lTexture)
                {
                    //display connected Material header only at the first time
                    if(pDisplayHeader){                    
                        //DisplayInt("    Textures connected to Material ", pMaterialIndex);
                        pDisplayHeader = false;
                    }             
  
                    DisplayTextureInfo(lTexture, -1);
                }
            }
        }
    }//end if pProperty

}


void DisplayTextureInfo(FbxTexture* pTexture, int pBlendMode)
{
    FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(pTexture);
    FbxProceduralTexture *lProceduralTexture = FbxCast<FbxProceduralTexture>(pTexture);

    //DisplayString("            Name: \"", (char *) pTexture->GetName(), "\"");
    if (lFileTexture)
    {
		std::cout << "**filename: " << lFileTexture->GetFileName() << "\n";
        //DisplayString("            Type: File Texture");
        //DisplayString("            File Name: \"", (char *) lFileTexture->GetFileName(), "\"");
    }
    else if (lProceduralTexture)
    {
        //DisplayString("            Type: Procedural Texture");
    }
    //DisplayDouble("            Scale U: ", pTexture->GetScaleU());
    //DisplayDouble("            Scale V: ", pTexture->GetScaleV());
    //DisplayDouble("            Translation U: ", pTexture->GetTranslationU());
    //DisplayDouble("            Translation V: ", pTexture->GetTranslationV());
    //DisplayBool("            Swap UV: ", pTexture->GetSwapUV());
    //DisplayDouble("            Rotation U: ", pTexture->GetRotationU());
    //DisplayDouble("            Rotation V: ", pTexture->GetRotationV());
    //DisplayDouble("            Rotation W: ", pTexture->GetRotationW());

    char* lAlphaSources[] = { "None", "RGB Intensity", "Black" };

    //DisplayString("            Alpha Source: ", lAlphaSources[pTexture->GetAlphaSource()]);
    //DisplayDouble("            Cropping Left: ", pTexture->GetCroppingLeft());
    //DisplayDouble("            Cropping Top: ", pTexture->GetCroppingTop());
    //DisplayDouble("            Cropping Right: ", pTexture->GetCroppingRight());
    //DisplayDouble("            Cropping Bottom: ", pTexture->GetCroppingBottom());

    char* lMappingTypes[] = { "Null", "Planar", "Spherical", "Cylindrical", 
        "Box", "Face", "UV", "Environment" };

    //DisplayString("            Mapping Type: ", lMappingTypes[pTexture->GetMappingType()]);

    if (pTexture->GetMappingType() == FbxTexture::ePlanar)
    {
        char* lPlanarMappingNormals[] = { "X", "Y", "Z" };

        //DisplayString("            Planar Mapping Normal: ", lPlanarMappingNormals[pTexture->GetPlanarMappingNormal()]);
    }

    char* lBlendModes[]   = { "Translucent", "Add", "Modulate", "Modulate2" };   
    if(pBlendMode >= 0)
        //DisplayString("            Blend Mode: ", lBlendModes[pBlendMode]);
    //DisplayDouble("            Alpha: ", pTexture->GetDefaultAlpha());

    if (lFileTexture)
    {
        char* lMaterialUses[] = { "Model Material", "Default Material" };
        //DisplayString("            Material Use: ", lMaterialUses[lFileTexture->GetMaterialUse()]);
    }

    char* pTextureUses[] = { "Standard", "Shadow Map", "Light Map", 
        "Spherical Reflexion Map", "Sphere Reflexion Map", "Bump Normal Map" };

    //DisplayString("            Texture Use: ", pTextureUses[pTexture->GetTextureUse()]);
    //DisplayString("");                

}





Mesh* loader::load3ds(ID3D10Device* device, char* filename)
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
	if ((l_file=fopen (filename, "rb"))== NULL) return NULL; //Open the file
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
			//cout << "********Vertices******** \n";
			for(i=0; i<l_qty; i++)
			{
				fread(&mVertices[i].pos.x, sizeof(float), 1, l_file); //cout << mVertices[i].pos.x << " ";
				fread(&mVertices[i].pos.y, sizeof(float), 1, l_file); //cout << mVertices[i].pos.y << " ";
				fread(&mVertices[i].pos.z, sizeof(float), 1, l_file); //cout << mVertices[i].pos.z << "\n";

			}
			break;

		case 0x4120: //Face Description chunk (read indices)
			fread(&l_qty, sizeof(unsigned short), 1, l_file);
			mNumFaces = l_qty;
			mIndices = new DWORD[l_qty*3];
			tempIndices = new unsigned short[l_qty*3];
			attributeBuffer = new UINT[l_qty];
			//cout << "\n *********Indices********* " << mNumFaces << "\n";
			for(i=0; i<l_qty; i++)
			{
				fread(&tempIndices[i*3+0], sizeof(unsigned short), 1, l_file);
				fread(&tempIndices[i*3+1], sizeof(unsigned short), 1, l_file);
				fread(&tempIndices[i*3+2], sizeof(unsigned short), 1, l_file);
				fread(&l_face_flags, sizeof(unsigned short), 1, l_file);
				attributeBuffer[i] = 0;

				mIndices[i*3+0] = tempIndices[i*3+0]; //cout << mIndices[i*3+0] << " ";
				mIndices[i*3+1] = tempIndices[i*3+1]; //cout << mIndices[i*3+1] << " ";
				mIndices[i*3+2] = tempIndices[i*3+2]; //cout << mIndices[i*3+2] << "\n ";
			}
			break;

		case 0x4140: //Mapping Coordinates List chunk (read texture coordinates)
			fread(&l_qty, sizeof(unsigned short), 1, l_file);
			//std::cout<<"\n\ntesting";
			for(i=0; i<l_qty; i++)
			{
				fread(&mVertices[i].texC.x, sizeof(float), 1, l_file); //std::cout<<mVertices[i].texC.x<<" ";
				fread(&mVertices[i].texC.y, sizeof(float), 1, l_file); //std::cout<<mVertices[i].texC.y<<"\n";

				mVertices[i].texC.y = -mVertices[i].texC.y;
				mVertices[i].texC.x = -mVertices[i].texC.x;
			}
			break;

		default:
			fseek(l_file, l_chunk_length-6, SEEK_CUR);
		}
	}

	fclose(l_file);


	//Calculate normals and tangents

	D3DXVECTOR3 *tan1 = new D3DXVECTOR3[mNumFaces * 3];
	D3DXVECTOR3 *tan2 = tan1 + mNumVertices;
	ZeroMemory(tan1, mNumVertices * sizeof(D3DXVECTOR3) * 2);

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

		// Normal calculation
		// Compute the normal of the triangle by taking the cross of two sides of the triangle
		D3DXVECTOR3 e0 = v1.pos - v0.pos;
		D3DXVECTOR3 e1 = v2.pos - v0.pos;

		D3DXVECTOR3 faceNormal;
		D3DXVec3Cross(&faceNormal, &e0, &e1);

		// Add the computed normal to each vertex of the triangle
		mVertices[i0].normal += faceNormal;
		mVertices[i1].normal += faceNormal;
		mVertices[i2].normal += faceNormal;

		// Tangent calculation (partly)
		float x1 = v1.pos.x - v0.pos.x;
		float x2 = v2.pos.x - v0.pos.x;
		float y1 = v1.pos.y - v0.pos.y;
		float y2 = v2.pos.y - v0.pos.y;
		float z1 = v1.pos.z - v0.pos.z;
		float z2 = v2.pos.z - v0.pos.z;

		float s1 = v1.texC.x - v0.texC.x;
		float s2 = v2.texC.x - v0.texC.x;
		float t1 = v1.texC.y - v0.texC.y;
		float t2 = v2.texC.y - v0.texC.y;

		float r = 1.0f / (s1 * t2 - s2 * t1);
		D3DXVECTOR3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                (t2 * z1 - t1 * z2) * r);
		D3DXVECTOR3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                (s1 * z2 - s2 * z1) * r);

		tan1[i0] += sdir;
		tan1[i1] += sdir;
		tan1[i2] += sdir;

		tan2[i0] += tdir;
		tan2[i1] += tdir;
		tan2[i2] += tdir;
	}

	// Finish normal and tangent calculations
	for(DWORD j = 0; j < mNumVertices; ++j)
	{
		// Normalize the added normals for each vertex
		D3DXVec3Normalize(&mVertices[j].normal, &mVertices[j].normal);

		// Calculate tangents
		D3DXVECTOR3 n = mVertices[j].normal;
		D3DXVECTOR3 t = tan1[j];

		mVertices[j].tangent = (t - n * D3DXVec3Dot(&n, &t));
		D3DXVec3Normalize(&mVertices[j].tangent, &mVertices[j].tangent);
	}

	delete[] tan1;

	cout << "\nmNumVertices " << mNumVertices << "\n";
	cout << "mNumberFaces " << mNumFaces << "\n\n";


	//Load data read from .3ds file into Mesh
	Mesh* mMesh = new Mesh();
	ID3DX10Mesh* mMeshData = NULL;

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

	mMesh->fileType = "3ds";
	mMesh->mMeshData = mMeshData;

	return mMesh;
}