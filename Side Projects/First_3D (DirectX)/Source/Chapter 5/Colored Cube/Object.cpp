#include "Object.h"
#include "Effects.h"
#include "Loader.h"
#include <istream>
#include <fstream>
#include <iostream>
#include "Vertex.h"

using namespace std;


wistream& operator>>(wistream& is, D3DXVECTOR3& v)
{
	is >> v.x >> v.y >> v.z;
	return is;
}

wistream& operator>>(wistream& is, D3DXVECTOR2& v)
{
	is >> v.x >> v.y;
	return is;
}



Object::Object()
: md3dDevice(0), mTranslation(0.0f,0.0f,0.0f), mRotation(0.0f,0.0f,0.0f), mScaling(1.0f,1.0f,1.0f)
{
}

Object::~Object()
{
	mMeshs.clear();

	debugOutputFile.close();
}

void Object::initObject(ID3D10Device* device)
{
	md3dDevice = device;
	getFXHandles();
	//load3ds::load(device, mMeshData, filename);
}

void Object::createTexturesAll(
	std::wstring diffuseName,
	std::wstring alphaName,
	std::wstring specularName,
	std::wstring normalName)
{
	for(int i = 0; i < mMeshs.size(); ++i)
	{
		createTexturesAt(i,diffuseName,alphaName,specularName,normalName);
	}
}

void Object::createTexturesAt(
	int index,
	std::wstring diffuseName,
	std::wstring alphaName,
	std::wstring specularName,
	std::wstring normalName)
{
	if(index < mMeshs.size())
	{
		mMeshs.at(index)->mDiffuseTexture  = createTex(diffuseName);
		mMeshs.at(index)->mAlphaTexture    = createTex(alphaName);
		mMeshs.at(index)->mSpecularTexture = createTex(specularName);
		mMeshs.at(index)->mNormalTexture   = createTex(normalName);
	}
}

void Object::setEyePos(const D3DXVECTOR3& eyePos)
{
	mfxEyePosVar->SetRawValue((void**)&eyePos, 0, sizeof(D3DXVECTOR3));
}

void Object::setLights(Light* lights, int numLights)
{
	mLights = lights;
	mNumLights = numLights;
}

void Object::setCubeMap(ID3D10ShaderResourceView* cubeMap)
{
	mCubeMap = cubeMap;
}

void Object::translate(float x, float y, float z)
{
	mTranslation.x += x;
	mTranslation.y += y;
	mTranslation.z += z;
}

void Object::rotate(float x, float y, float z)
{
	mRotation.x += x;
	mRotation.y += y;
	mRotation.z += z;
}

void Object::scale(float x, float y, float z)
{
	mScaling.x += x;
	mScaling.y += y;
	mScaling.z += z;
}

void Object::draw(const D3DXMATRIX& view, const D3DXMATRIX& proj)
{
	for(int j = 0; j < mMeshs.size(); ++j)
	{
		Mesh* mesh = mMeshs.at(j);

		//Calculate transformations
		D3DXMATRIX world;
		D3DXMATRIX T, Rx, Ry, Rz, S;

		D3DXMatrixTranslation(&T, mTranslation.x, mTranslation.y, mTranslation.z);
		D3DXMatrixRotationX(&Rx, mRotation.x);
		D3DXMatrixRotationY(&Ry, mRotation.y);
		D3DXMatrixRotationZ(&Rz, mRotation.z);
		D3DXMatrixScaling(&S, mScaling.x, mScaling.y, mScaling.z);

		world = S*Rx*Ry*Rz*T;

		D3DXMATRIX viewTemp = view;
		viewTemp._31 = -viewTemp._31;
		viewTemp._32 = -viewTemp._32;
		viewTemp._33 = -viewTemp._33;
		viewTemp._34 = -viewTemp._34;

		D3DXMATRIX WVP = world*view*proj;

		mfxWVPVar->SetMatrix((float*)&WVP);
		mfxWorldVar->SetMatrix((float*)&world);

		//WARNGING: Set up lights -> Do for loop later!
		if(mNumLights > 0)
		{
			mfxLightVar->SetRawValue((void**)&mLights[0], 0, sizeof(Light));
		}
		if(mNumLights > 1)
		{
			mfxLightVar2->SetRawValue((void**)&mLights[1], 0, sizeof(Light));
		}

		//Set cube map
		mfxCubeMapVR->SetResource(mCubeMap);

		D3D10_TECHNIQUE_DESC techDesc;
		mTech->GetDesc( &techDesc );

		UINT subsetID = 0;
		for(UINT i = 0; i < techDesc.Passes; ++i)
		{
			ID3D10EffectPass* pass = mTech->GetPassByIndex(i);

			mfxDiffuseMapVar->SetResource(mesh->mDiffuseTexture);
			mfxAlphaMapVar->SetResource(mesh->mAlphaTexture);
			mfxSpecularMapVar->SetResource(mesh->mSpecularTexture);
			mfxNormalMapVR->SetResource(mesh->mNormalTexture);

			mfxReflectEnabledVar->SetBool(false);

			//remove later!
			D3DXMATRIX texMtx;
			D3DXMatrixIdentity(&texMtx);
			mfxTexMtxVar->SetMatrix((float*)&texMtx);

			pass->Apply(0);
			mesh->mMeshData->DrawSubset(subsetID);
		}
	}
}

void Object::getFXHandles()
{
	mTech                = fx::MeshFX->GetTechniqueByName("ColorTech");
	mfxLightVar          = fx::MeshFX->GetVariableByName("gLight");
	mfxLightVar2         = fx::MeshFX->GetVariableByName("gLight2");
	mfxDiffuseMapVar     = fx::MeshFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxAlphaMapVar       = fx::MeshFX->GetVariableByName("gAlphaMap")->AsShaderResource();
	mfxSpecularMapVar    = fx::MeshFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxCubeMapVR         = fx::MeshFX->GetVariableByName("gCubeMap")->AsShaderResource();
	mfxNormalMapVR       = fx::MeshFX->GetVariableByName("gNormalMap")->AsShaderResource();
	mfxReflectEnabledVar = fx::MeshFX->GetVariableByName("reflectEnabled")->AsScalar();
	mfxTexMtxVar         = fx::MeshFX->GetVariableByName("gTexMtx")->AsMatrix();
	mfxEyePosVar         = fx::MeshFX->GetVariableByName("gEyePosW");
	mfxWorldVar          = fx::MeshFX->GetVariableByName("gWorld")->AsMatrix();
	mfxWVPVar            = fx::MeshFX->GetVariableByName("gWVP")->AsMatrix();
}

ID3D10ShaderResourceView* Object::createTex(std::wstring filename)
{
	ID3D10ShaderResourceView* rv = 0;
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, filename.c_str(), 0, 0, &rv, 0 ));

	return rv;
}

ID3D10ShaderResourceView* Object::createCubeTex(ID3D10Device* device, std::wstring filename)
{
	D3DX10_IMAGE_LOAD_INFO loadInfo;
    loadInfo.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

	ID3D10Texture2D* tex = 0;
	HR(D3DX10CreateTextureFromFile(device, filename.c_str(), 
		&loadInfo, 0, (ID3D10Resource**)&tex, 0) );

    D3D10_TEXTURE2D_DESC texDesc;
	tex->GetDesc(&texDesc);

    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
    viewDesc.TextureCube.MipLevels = texDesc.MipLevels;
    viewDesc.TextureCube.MostDetailedMip = 0;
    
	ID3D10ShaderResourceView* rv = 0;
	HR(device->CreateShaderResourceView(tex, &viewDesc, &rv));
   
	ReleaseCOM(tex);

	return rv;
}

bool Object::load(ID3D10Device* device, string filename)
{
	// Convert string to *char
	char* cFileName = new char[filename.size()+1];
	cFileName[filename.size()] = 0;
	memcpy(cFileName, filename.c_str(), filename.size());

	// Determine type of file
	string fileExtension = filename.substr(filename.find_last_of(".") + 1);
	if(fileExtension == "3ds")
	{
		Mesh* meshData = NULL;
		if( (meshData = loader::load3ds(device, cFileName)) != NULL )
		{
			mMeshs.push_back(meshData);
			return true;
		}
		std::cout << "Object::load Error: Could not load .3ds file";
	}
	else if(fileExtension == "fbx")
	{
		std::vector<Mesh*> tempMeshs = loader::loadFbx(device, filename.c_str());
		int num = 0;
		for(int i = 0; i < tempMeshs.size(); ++i)
		{
			cout << i << " " << tempMeshs.at(i)->name << "\n";
			mMeshs.push_back(tempMeshs.at(i));
			num++;
		}
		cout << "num processed " << num << "\n";
		return true;
	}

	std::cout << "Object::load Error: Unsupported file format/extension";
	return false;
}