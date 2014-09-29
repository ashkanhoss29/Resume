//=============================================================================
// Color Cube App.cpp by Frank Luna (C) 2008 All Rights Reserved.
//
// Demonstrates coloring.
//
// Controls:
//		'A'/'D'/'W'/'S' - Rotate 
//
//=============================================================================

#include "..\..\Common\d3dApp.h"
#include "Vertex.h"
#include "Light.h"
#include "Effects.h"
#include "Object.h"
#include "Box.h"
#include "Plane.h"
#include "Sky.h"
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <XInput.h>
#include <dinput.h>

const int fireFrameCount = 120;

class ColoredCubeApp : public D3DApp
{
public:
	ColoredCubeApp(HINSTANCE hInstance);
	~ColoredCubeApp();

	void initApp();
	void initInput();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 

private:
	void buildFX();
	void buildVertexLayouts();
 
private:
	
	Box mBox;
	Plane mPlane;
	Sky mSky;
	Object mTree;
	Object mObjBox;

	Light mLights[2];

	ID3D10InputLayout* mVertexLayout;
	ID3D10ShaderResourceView* mFireAnimationMapRVs[fireFrameCount];
	ID3D10ShaderResourceView* mCrateMapRV;
	ID3D10ShaderResourceView* mGrassMapRV;
	ID3D10ShaderResourceView* mSpecularMapRV;
	ID3D10ShaderResourceView* mCubeMapRV;
	ID3D10ShaderResourceView* mDefaultNormalMapRV;
	ID3D10ShaderResourceView* mBrickNormalMapRV;

	float animationTimeElapsed;
	float animationTimePrev;
	int frameOfAnimation;

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10EffectVariable* mfxLightVar2;
	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecularMapVar;
	ID3D10EffectShaderResourceVariable* mfxCubeMapVR;
	ID3D10EffectShaderResourceVariable* mfxNormalMapVR;
	ID3D10EffectScalarVariable* mfxReflectEnabledVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;

	LPDIRECTINPUT8 lpdi;
	LPDIRECTINPUTDEVICE8 lpdiMouse;
	DIMOUSESTATE mouseState;

	D3DXMATRIX mBoxWorld;
	D3DXMATRIX mPlaneWorld;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mWVP;

	D3DXVECTOR3 mCameraPos;

	float mTheta;
	float mPhi;

	std::ofstream myOutFile;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	ColoredCubeApp theApp(hInstance);
	
	theApp.initApp();
	//theApp.initInput();

	return theApp.run();
}

ColoredCubeApp::ColoredCubeApp(HINSTANCE hInstance)
: D3DApp(hInstance), mFX(0), mTech(0), mVertexLayout(0),
  mfxWVPVar(0), mfxWorldVar(0), mfxEyePosVar(0), mfxLightVar(0), mfxLightVar2(0),
  mCrateMapRV(0), mGrassMapRV(0), mSpecularMapRV(0), mfxDiffuseMapVar(0), mfxSpecularMapVar(0), mfxTexMtxVar(0),
  mTheta(0.0f), mPhi(PI*0.5f), animationTimeElapsed(0.0f), animationTimePrev(0.0f), frameOfAnimation(0),
  mCameraPos(0.0f,0.0f,0.0f)
{
	D3DXMatrixIdentity(&mBoxWorld);
	D3DXMatrixIdentity(&mPlaneWorld);
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mWVP);

	for (int i = 0; i < fireFrameCount; i++)
	{
		mFireAnimationMapRVs[i] = NULL;
	}

	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
}

ColoredCubeApp::~ColoredCubeApp()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
	ReleaseCOM(mCrateMapRV);
	ReleaseCOM(mSpecularMapRV);

	for( int i = 0; i < fireFrameCount; i++)
	{
		ReleaseCOM(mFireAnimationMapRVs[i]);
	}

	myOutFile.close();

	fclose(stdout);
	FreeConsole();
}

void ColoredCubeApp::initApp()
{
	D3DApp::initApp();
	
	myOutFile.open("debug.txt");

	fx::InitAll(md3dDevice);
	
	mBox.init(md3dDevice, 1.0f);
	mPlane.init(md3dDevice, 1.0f); //we send scale of 1.0f what does that mean??

	
	mTree.initObject(md3dDevice);
	std::string treeFileName = "MediumPolyTree.3ds";
	mTree.load(md3dDevice, treeFileName);
	mTree.createTexturesAll(L"LeafCol.jpg", L"LeafAlpha.jpg", L"mySpec.jpg", L"LeafnormalX_normals.PNG");
	mTree.setCubeMap(Object::createCubeTex(md3dDevice, L"grassenvmap1024.dds"));
	mTree.rotate(-1.5f,0.0f,0.0f);
	mTree.translate(3.0f,-3.0f,-2.6f);

	mObjBox.initObject(md3dDevice);
	std::string boxFileName = "man2.fbx";
	mObjBox.load(md3dDevice, boxFileName);
	mObjBox.createTexturesAll( L"manD.jpg", L"defaultAlpha.jpg", L"defaultspec.dds", L"manN.jpg");
	mObjBox.createTexturesAt(0, L"bricks.dds", L"defaultAlpha.jpg", L"spec.dds", L"womanHairN.jpg");
	mObjBox.setCubeMap(Object::createCubeTex(md3dDevice, L"grassenvmap1024.dds"));
	mObjBox.rotate(0.0f,0.0f,3.14f);
	mObjBox.translate(-3.0f,0.0f,2.5f);
	mObjBox.scale(2.0f,2.0f,2.0f);
	
	
	buildFX();
	buildVertexLayouts();

	//mLights[0].dir      = D3DXVECTOR3(0.57735f, -0.57735f, 0.57735f);
	mLights[0].dir      = D3DXVECTOR3(0.576f, -0.576f, -0.576f);
	mLights[0].ambient  = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mLights[0].diffuse  = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mLights[0].specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

	// Pointlight--position is changed every frame to animate.
	mLights[1].pos      = D3DXVECTOR3(2.0f,2.0f,2.0f);
	mLights[1].ambient  = D3DXCOLOR(0.8f, 0.8f, 0.0f, 1.0f);
	mLights[1].diffuse  = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	mLights[1].specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	mLights[1].att.x    = 0.0f;
	mLights[1].att.y    = 1.0f;
	mLights[1].att.z    = 0.0f;
	mLights[1].range    = 120.0f;

	animationTimeElapsed = 0.0f;
	animationTimePrev = mTimer.getGameTime();

	for(int i = 0; i < fireFrameCount; i++)
	{
		std::wostringstream fileName;

		fileName << L"FireAnim\\Fire";

		if(i+1 < 10)
			fileName << L"00";
		else if(i+1 < 100)
			fileName << L"0";

		fileName << i+1 << L".bmp";

		std::wstring wbuffer = fileName.str();
		LPCWSTR usableName = wbuffer.c_str();
		
		HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice,
			usableName, 0, 0, &mFireAnimationMapRVs[i], 0));
	}

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice,
		L"bricks.dds", 0, 0, &mCrateMapRV, 0 ));
	
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice,
		L"bricks_normal.dds", 0, 0, &mDefaultNormalMapRV, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice,
		L"stone_diffuse.dds", 0, 0, &mGrassMapRV, 0 ));
	
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice,
		L"stone_normal.dds", 0, 0, &mBrickNormalMapRV, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice,
		L"spec.dds", 0, 0, &mSpecularMapRV, 0 ));

	

	

	// If not, create it.
	D3DX10_IMAGE_LOAD_INFO loadInfo;
    loadInfo.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

	ID3D10Texture2D* tex = 0;
	HR(D3DX10CreateTextureFromFile(md3dDevice, L"grassenvmap1024.dds", 
		&loadInfo, 0, (ID3D10Resource**)&tex, 0) );

    D3D10_TEXTURE2D_DESC texDesc;
	tex->GetDesc(&texDesc);

    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
    viewDesc.TextureCube.MipLevels = texDesc.MipLevels;
    viewDesc.TextureCube.MostDetailedMip = 0;
    
	HR(md3dDevice->CreateShaderResourceView(tex, &viewDesc, &mCubeMapRV));
   
	ReleaseCOM(tex);

	mSky.init(md3dDevice, mCubeMapRV, 5000.0f);
}

void ColoredCubeApp::initInput()
{
	DirectInput8Create(
		mhAppInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&lpdi,
		NULL);

	lpdi->CreateDevice(GUID_SysMouseEm, &lpdiMouse, NULL);
	lpdiMouse->SetDataFormat(&c_dfDIMouse);
	lpdiMouse->SetCooperativeLevel(mhMainWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
}

void ColoredCubeApp::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 1000.0f); //vertical fov is pi/4 or 45 degrees
}

void ColoredCubeApp::updateScene(float dt)
{
	D3DApp::updateScene(dt);

	if(GetAsyncKeyState(VK_ESCAPE)) PostQuitMessage(0);

	float moveRate = 1.0f;
	if(GetAsyncKeyState(VK_LSHIFT))
		moveRate *= 2.0f;
	if(GetAsyncKeyState(VK_LCONTROL))
		moveRate *= 0.1f;


	//Move camera with mouse
	if(!createdOriginPos)
	{
		mCursorOriginPos.x = 100;
		mCursorOriginPos.y = 100;
		ClientToScreen(mhMainWnd, &mCursorOriginPos);
		SetCursorPos(mCursorOriginPos.x, mCursorOriginPos.y);
		ShowCursor(false);
		createdOriginPos = true;
	}

	GetCursorPos(&mCursorPos);
	float myX = ((float)mCursorOriginPos.x - (float)mCursorPos.x);
	float myY = ((float)mCursorOriginPos.y - (float)mCursorPos.y);

	//if(dt < 0.01f)
	{
		mTheta += myX * 0.005f;//  * dt * 4.0f;
		mPhi   -= myY * 0.005f;//  * dt * 4.0f;
		//myOutFile  << std::fixed << std::setprecision(4) << dt << " " << mTheta << " " << mPhi << "\n";
	}

	SetCursorPos(mCursorOriginPos.x, mCursorOriginPos.y);


	//Move camera with gamepad
	XINPUT_STATE state;
	ZeroMemory( &state, sizeof(XINPUT_STATE) );
	DWORD gamepadConnected = XInputGetState(0, &state);

	if( gamepadConnected == ERROR_SUCCESS )
	{
		float RX = state.Gamepad.sThumbRX;
		float RY = state.Gamepad.sThumbRY;
		float magnitude = sqrt(RX*RX + RY*RY);
		if(magnitude > 10000.0f)
		{
			mTheta += RX * dt * -0.0001f;
			mPhi   += RY * dt * -0.00005f;
		}
	}


	// Move camera with keyboard
	// Update angles based on input to orbit camera around box.
	if(GetAsyncKeyState('J') & 0x8000)	mTheta += 2.0f*dt;
	if(GetAsyncKeyState('L') & 0x8000)	mTheta -= 2.0f*dt;
	if(GetAsyncKeyState('I') & 0x8000)	mPhi -= 2.0f*dt;
	if(GetAsyncKeyState('K') & 0x8000)	mPhi += 2.0f*dt;

	// Restrict the angle mPhi.
	if( mPhi < 0.1f )	mPhi = 0.1f;
	if( mPhi > PI-0.1f)	mPhi = PI-0.1f;
	
	// Convert Spherical to Cartesian coordinates: mPhi measured from +y
	// and mTheta measured counterclockwise from -z.

	D3DXVECTOR3 targetPos(
		5.0f*sinf(mPhi)*sinf(mTheta),
		5.0f*cosf(mPhi),
		-5.0f*sinf(mPhi)*cosf(mTheta)
		);


	if(gamepadConnected == ERROR_SUCCESS)
	{
		// Move the camera with the left thumbstick
		float LX = state.Gamepad.sThumbLX;
		float LY = state.Gamepad.sThumbLY;
		float magnitude = sqrt(LX*LX + LY*LY);
		if(magnitude > 10000.0f)
		{
			// Forward and Backward movement added to move; directionToMoveForwardBackward
			D3DXVECTOR3 directionToMoveForwardBackward(0.0f,0.0f,0.0f);
			directionToMoveForwardBackward = targetPos;
			D3DXVec3Normalize(&directionToMoveForwardBackward, &directionToMoveForwardBackward);
			directionToMoveForwardBackward *= state.Gamepad.sThumbLY * dt * 0.001f;
			mCameraPos += directionToMoveForwardBackward;

			// Right and Left movement added to move; directionToMoveRightLeft
			D3DXVECTOR3 directionToMoveRightLeft(0.0f,0.0f,0.0f);
			D3DXVECTOR3 pointAboveTarget(
				5.0f*sinf(mPhi)*sinf(mTheta),
				5.0f*cosf(mPhi)+0.1f,
				-5.0f*sinf(mPhi)*cosf(mTheta)
				);
			D3DXVec3Cross(&directionToMoveRightLeft, &targetPos, &pointAboveTarget);
			D3DXVec3Normalize(&directionToMoveRightLeft, &directionToMoveRightLeft);
			directionToMoveRightLeft *= state.Gamepad.sThumbLX * dt * -0.001f;
			mCameraPos +=directionToMoveRightLeft;
		}
	}


	if( (GetAsyncKeyState('D') & 0x8000) ||
		(GetAsyncKeyState('A') & 0x8000) ||
		(GetAsyncKeyState('W') & 0x8000) ||
		(GetAsyncKeyState('S') & 0x8000) ||
		(gamepadConnected == ERROR_SUCCESS && state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ||
		(gamepadConnected == ERROR_SUCCESS && state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)  ||
		(gamepadConnected == ERROR_SUCCESS && state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)    ||
		(gamepadConnected == ERROR_SUCCESS && state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		)
	{
		D3DXVECTOR3 move(0.0f,0.0f,0.0f);

		if((GetAsyncKeyState('D') & 0x8000) || (gamepadConnected == ERROR_SUCCESS && state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			D3DXVECTOR3 pointAboveTarget(
				5.0f*sinf(mPhi)*sinf(mTheta),
				5.0f*cosf(mPhi)+0.1f,
				-5.0f*sinf(mPhi)*cosf(mTheta)
				);
			D3DXVECTOR3 directionToMove(0.0f,0.0f,0.0f);
			D3DXVec3Cross(&directionToMove, &targetPos, &pointAboveTarget);
			D3DXVec3Normalize(&directionToMove, &directionToMove);
			move = move - directionToMove;
		}
		if((GetAsyncKeyState('A') & 0x8000) || (gamepadConnected == ERROR_SUCCESS && state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))
		{
			D3DXVECTOR3 pointAboveTarget(
				5.0f*sinf(mPhi)*sinf(mTheta),
				5.0f*cosf(mPhi)+0.1f,
				-5.0f*sinf(mPhi)*cosf(mTheta)
				);
			D3DXVECTOR3 directionToMove(0.0f,0.0f,0.0f);
			D3DXVec3Cross(&directionToMove, &targetPos, &pointAboveTarget);
			D3DXVec3Normalize(&directionToMove, &directionToMove);
			move = move + directionToMove;
		}
		if((GetAsyncKeyState('W') & 0x8000) || (gamepadConnected == ERROR_SUCCESS && state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP))
		{
			D3DXVECTOR3 directionToMove(0.0f,0.0f,0.0f);
			directionToMove = targetPos;
			D3DXVec3Normalize(&directionToMove, &directionToMove);
			move = move + directionToMove;
		}
		if((GetAsyncKeyState('S') & 0x8000) || (gamepadConnected == ERROR_SUCCESS && state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN))
		{
			D3DXVECTOR3 directionToMove(0.0f,0.0f,0.0f);
			directionToMove = targetPos;
			D3DXVec3Normalize(&directionToMove, &directionToMove);
			move = move - directionToMove;
		}


		D3DXVec3Normalize(&move, &move);
		mCameraPos += move * dt * 20.0f * moveRate;
	}

	if(GetAsyncKeyState('1'))
	{
		//mLights[1].specular = BLUE;
		mLights[1].pos = mCameraPos;
	}
	if(GetAsyncKeyState('2'))
	{
		mLights[1].specular = GREEN;
	}
	
	
	targetPos = targetPos + mCameraPos;

	// Build the view matrix.
	//D3DXVECTOR3 pos(cameraPos.x, cameraPos.y, cameraPos.z);
	//D3DXVECTOR3 target(mCameraVec.x, mCameraVec.y, mCameraVec.z);
	D3DXVECTOR3 target(targetPos.x, targetPos.y, targetPos.z);
	D3DXVECTOR3 pos(mCameraPos.x, mCameraPos.y, mCameraPos.z);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);
}

void ColoredCubeApp::drawScene()
{
	D3DApp::drawScene();

	//Step through animation frame
	animationTimeElapsed += mTimer.getGameTime() - animationTimePrev;
	animationTimePrev = mTimer.getGameTime();
	if(animationTimeElapsed > 0.0666f)
	{
		animationTimeElapsed = 0.0f;
		frameOfAnimation++;
		if(frameOfAnimation > fireFrameCount-1)
		{
			frameOfAnimation = 0;
		}
	}

	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
    md3dDevice->IASetInputLayout(mVertexLayout);
    md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set per frame constants
	mfxEyePosVar->SetRawValue(&mCameraPos, 0, sizeof(D3DXVECTOR3));
	mfxLightVar->SetRawValue(&mLights[0], 0, sizeof(Light));
	mfxLightVar2->SetRawValue(&mLights[1], 0, sizeof(Light));

	mfxCubeMapVR->SetResource(mCubeMapRV);
   
	// set constants
	mWVP = mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP); //set gWVP in color.fx to mWVP

	mTree.setEyePos(mCameraPos);
	mTree.setLights(mLights, 2);
	mTree.draw(mView, mProj);
	mObjBox.setEyePos(mCameraPos);
	mObjBox.setLights(mLights, 2);
	mObjBox.draw(mView, mProj);

    D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        ID3D10EffectPass* pass = mTech->GetPassByIndex( p ); //zero is always used in D3D10
		D3DXMATRIX texMtx;
        
		mWVP = mBoxWorld*mView*mProj;
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mfxWorldVar->SetMatrix((float*)&mBoxWorld);
		mfxDiffuseMapVar->SetResource(mCrateMapRV);
		//mfxDiffuseMapVar->SetResource(mFireAnimationMapRVs[frameOfAnimation]);
		mfxSpecularMapVar->SetResource(mSpecularMapRV);
		mfxNormalMapVR->SetResource(mDefaultNormalMapRV);
		mfxReflectEnabledVar->SetBool(false);
		D3DXMatrixIdentity(&texMtx);
		mfxTexMtxVar->SetMatrix((float*)&texMtx);
		pass->Apply(0);
		mBox.draw();

		mWVP = mPlaneWorld*mView*mProj;
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mfxWorldVar->SetMatrix((float*)&mPlaneWorld);
		mfxDiffuseMapVar->SetResource(mGrassMapRV);
		mfxNormalMapVR->SetResource(mBrickNormalMapRV);
		mfxReflectEnabledVar->SetBool(true);
		D3DXMATRIX s;
		D3DXMatrixScaling(&s, 5.0f, 5.0f, 1.0f);
		texMtx = s;
		D3DXMatrixIdentity(&texMtx);
		mfxTexMtxVar->SetMatrix((float*)&texMtx);
		pass->Apply(0);
		mPlane.draw();
    }

	mSky.draw(mWVP);

	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	md3dDevice->RSSetState(0);
	mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);

	mSwapChain->Present(0, 0);
}

void ColoredCubeApp::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
 
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"color.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &mFX, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	}

	mTech = mFX->GetTechniqueByName("ColorTech");
	
	mfxLightVar       = mFX->GetVariableByName("gLight");
	mfxLightVar2      = mFX->GetVariableByName("gLight2");
	mfxDiffuseMapVar  = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecularMapVar = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxCubeMapVR      = mFX->GetVariableByName("gCubeMap")->AsShaderResource();
	mfxNormalMapVR    = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
	mfxReflectEnabledVar = mFX->GetVariableByName("reflectEnabled")->AsScalar();
	mfxTexMtxVar      = mFX->GetVariableByName("gTexMtx")->AsMatrix();
	mfxEyePosVar      = mFX->GetVariableByName("gEyePosW");
	mfxWorldVar       = mFX->GetVariableByName("gWorld")->AsMatrix();
	mfxWVPVar         = mFX->GetVariableByName("gWVP")->AsMatrix();
}

void ColoredCubeApp::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 4, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}
 