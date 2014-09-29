#ifndef OBJECT_H
#define OBJECT_H

#include "..\..\Common\d3dUtil.h"
#include "Light.h"
#include "Mesh.h"
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <io.h> //filelength()
#include <iostream>


class Object
{
public:

	Object();
	~Object();


	void initObject(ID3D10Device* device); //used to be char* filename

	void loadm3d(ID3D10Device* device, std::wstring filename);
	bool load(ID3D10Device* device, std::string filename);
	//int load3ds(ID3D10Device* device, char* filename);

	void createTexturesAll(
		std::wstring diffuseName,
		std::wstring alphaName,
		std::wstring specularName,
		std::wstring normalName);

	void createTexturesAt(
		int index,
		std::wstring diffuseName,
		std::wstring alphaName,
		std::wstring specularName,
		std::wstring normalName);
	
	void setEyePos(const D3DXVECTOR3& eyePos);
	void setLights(Light* lights, int numLights);
	void setCubeMap(ID3D10ShaderResourceView* cubeMap);

	void translate(float x, float y, float z);
	void rotate(float x, float y, float z);
	void scale(float x, float y, float z);

	void draw(const D3DXMATRIX& view, const D3DXMATRIX& proj);

	static ID3D10ShaderResourceView* createCubeTex(ID3D10Device* device, std::wstring filename);

private:

	void getFXHandles();
	ID3D10ShaderResourceView* createTex(std::wstring filename);
	
	//Mesh
	ID3D10Device* md3dDevice;
	std::vector<Mesh*> mMeshs;
	

	D3DXVECTOR3 mTranslation;
	D3DXVECTOR3 mRotation;
	D3DXVECTOR3 mScaling;

	//ID3DX10Mesh* mMeshData;
	//ID3D10ShaderResourceView* mDiffuseTexture;
	//ID3D10ShaderResourceView* mAlphaTexture;
	//ID3D10ShaderResourceView* mSpecularTexture;
	//ID3D10ShaderResourceView* mNormalTexture;

	//Lights
	Light* mLights;
	int mNumLights;

	//Cube map (same acrros all objects)
	ID3D10ShaderResourceView* mCubeMap;

	ID3D10EffectTechnique* mTech;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10EffectVariable* mfxLightVar2;
	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxAlphaMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecularMapVar;
	ID3D10EffectShaderResourceVariable* mfxCubeMapVR;
	ID3D10EffectShaderResourceVariable* mfxNormalMapVR;
	ID3D10EffectScalarVariable* mfxReflectEnabledVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;


	/*
	UINT mNumSubsets;
	std::vector<D3DXVECTOR3> mReflectMtrls;
	std::vector<ID3D10ShaderResourceView*> mDiffuseTextures;
	std::vector<ID3D10ShaderResourceView*> mSpecTextures;
	std::vector<ID3D10ShaderResourceView*> mNormalTextures;
	*/



	std::ofstream debugOutputFile;
};

#endif