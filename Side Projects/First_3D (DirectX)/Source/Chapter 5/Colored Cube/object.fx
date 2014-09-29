//=============================================================================
// color.fx by Frank Luna (C) 2008 All Rights Reserved.
//
// Transforms and colors geometry.
//=============================================================================

struct Light
{
	float3 pos;
	float3 dir;
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 att;
	float  spotPower;
	float  range;
};

struct SurfaceInfo
{
	float3 pos;
	float3 normal;
	float4 diffuse;
	float4 spec;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWVP;
	float4x4 gTexMtx;
	bool reflectEnabled;
};

cbuffer cbPerFrame
{
	Light gLight;
	Light gLight2;
	float3 gEyePosW;
	//float reflectivity;
};

//Constant only for now, change this later!!
cbuffer cbFixed
{
	float gFogStart = 5.0f;
	float gFogRange = 180.0f;
	float3 gFogColor = (0.7f, 0.7f, 0.7f);
}

Texture2D gDiffuseMap;
Texture2D gAlphaMap;
Texture2D gSpecMap;
Texture2D gNormalMap;
TextureCube gCubeMap;

SamplerState gTriLinearSam
{
	//Filter = MIN_MAG_MIP_LINEAR;
	Filter = ANISOTROPIC;
	//MaxAnisotropy = 16;
};


struct VS_IN
{
	float3 posL     : POSITION;
	float3 tangentL : TANGENT;
	float3 normalL  : NORMAL;
	float2 texC     : TEXCOORD;
};

struct VS_OUT
{
	float4 posH     : SV_POSITION;
	float3 posW     : POSITION;
	float3 tangentW : TANGENT;
	float3 normalW  : NORMAL;
	float2 texC     : TEXCOORD;
	float  fogLerp   : FOG;
};


float3 ParallelLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	// Get the light vector, which is the opposite of the light rays
	float3 lightVec = -L.dir;

	// Add ambient term
	litColor += v.diffuse * L.ambient;

	float diffuseFactor = dot(lightVec, v.normal);
	[branch]
	if( diffuseFactor > 0.0f)
	{
		float  specPower  = max(v.spec.a, 1.0f);
		float3 toEye      = normalize(eyePos - v.pos);
		float3 R          = reflect(-lightVec, v.normal);
		float  specFactor = pow(max(dot(R, toEye), 0.0f), specPower);

		litColor += diffuseFactor * v.diffuse * L.diffuse;
		litColor += specFactor * v.spec * L.spec;
	}

	return litColor;
}

float3 PointLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	
	// The vector from the surface to the light.
	float3 lightVec = L.pos - v.pos;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	if( d > L.range )
		return float3(0.0f, 0.0f, 0.0f);
		
	// Normalize the light vector.
	lightVec /= d; 
	
	// Add the ambient light term.
	litColor += v.diffuse * L.ambient;	
	
	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	
	float diffuseFactor = dot(lightVec, v.normal);
	[branch]
	if( diffuseFactor > 0.0f )
	{
		float specPower   = max(v.spec.a, 1.0f);
		float3 toEye      = normalize(eyePos - v.pos);
		float3 reflectVec = reflect(-lightVec, v.normal);
		float specFactor  = pow(max(dot(reflectVec, toEye), 0.0f), specPower);
	
		// diffuse and specular terms
		litColor += diffuseFactor * v.diffuse * L.diffuse;
		litColor += specFactor * v.spec * L.spec;
	}
	
	// attenuate
	return litColor / dot(L.att, float3(1.0f, d, d*d));
}


float3 Refract( float3 I, float3 N, float etaRatio )
{
	float cosI = dot(-I, N);
	float cosT2 = 1.0f - etaRatio + etaRatio * (1.0f - cosI);
	float3 T = etaRatio * I + ((etaRatio * cosI - sqrt(abs(cosT2))) * N);
	return T * (float3)(cosT2 > 0);
}



VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;

	vOut.posW     = mul(float4(vIn.posL, 1.0f), gWorld);
	vOut.tangentW = mul(float4(vIn.tangentL, 0.0f), gWorld);
	vOut.normalW  = mul(float4(vIn.normalL, 0.0f), gWorld);

	// Transform to homogeneous clip space.
	 // Here we're multiplying a 4D vector with a 4x4 matrix
	 // The 4D vector is the input: the position of the vertex
	 // The 4x4 matrix is our combined World/View/Projection matrix
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWVP); //mul means multiply
	
	// Output vertex attributes for interpolation across triangle.
	vOut.texC = mul(float4(vIn.texC, 0.0f, 1.0f), gTexMtx);

	float d = distance(vOut.posW, gEyePosW);
	vOut.fogLerp = saturate( (d - gFogStart) / gFogRange );

	return vOut;
}


float4 PS(VS_OUT pIn) : SV_Target
{
	float4 diffuse = gDiffuseMap.Sample( gTriLinearSam, pIn.texC );
	float4 alpha   = gAlphaMap.Sample( gTriLinearSam, pIn.texC );
	float4 spec    = gSpecMap.Sample( gTriLinearSam, pIn.texC );
	float3 normalT = gNormalMap.Sample( gTriLinearSam, pIn.texC );

	clip(diffuse.a - 0.15f);
	clip(alpha.x - 0.15);

	spec.a *= 256.0f;

	normalT = 2.0f*normalT -1.0f;
    float3 N = normalize(pIn.normalW);
	float3 T = normalize(pIn.tangentW - dot(pIn.tangentW, N)*N);
	float3 B = cross(N,T);

	float3x3 TBN = float3x3(T, B, N);

	float3 bumpedNormalW = normalize(mul(normalT, TBN));

	SurfaceInfo v = {pIn.posW, bumpedNormalW, diffuse, spec};

	float3 litColor;

	litColor = ParallelLight(v, gLight, gEyePosW);
	litColor += PointLight(v, gLight2, gEyePosW);

	//Cube map reflections and refractions calculations
	[branch]
	if(reflectEnabled)
	{
		float reflectionAmount = 0.15f;
		float etaRatio = 1.333f;

		float3 I = pIn.posW - gEyePosW;
		float3 R = reflect(I, bumpedNormalW);
		float3 T = Refract(I, bumpedNormalW, etaRatio);

		float4 reflectColor = gCubeMap.Sample( gTriLinearSam, R );
		float4 refractColor = gCubeMap.Sample( gTriLinearSam, T );

		litColor = lerp(litColor, reflectColor, reflectionAmount);
	}


	//Fog calculation
	float3 foggedColor = lerp(litColor, gFogColor, pIn.fogLerp);//litColor;//lerp(litColor, gFogColor, pIn.fogLerp);

	return float4(foggedColor, diffuse.a);
}


RasterizerState rs
{
	FillMode = WIREFRAME;
};

technique10 ColorTech
{
    pass P0
    {
		//SetRasterizerState(rs);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
