cbuffer cbPlayerInfo : register(b0)
{
	matrix		gmtxPlayerWorld : packoffset(c0);
};

cbuffer cbCameraInfo : register(b1)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix		gmtxGameObject : packoffset(c0);
	uint		terrainID : packoffset(c4);

};
//struct GAMEOBJECTINFO
//{
//	matrix		m_mtxGameObject
//};
struct INSTANCEDGAMEOBJECTINFO
{
	matrix m_mtxGameObject;
};
//StructuredBuffer<GAMEOBJECTINFO> gGameObjectInfos : register(t0); //gGameObjectInfos 변수는 배열이야 이런뜻임, gGameObjectInfos 를 배열처럼써도됨, 근데 사이즈가없는데? -> 버퍼만들고 최대사이즈 정해놓으면 max사이즈까지면 접근하ㅕㄴ됨,
StructuredBuffer<INSTANCEDGAMEOBJECTINFO> gGameObjectInfos : register(t0);
// todo 레지스터 텍스쳐랑 따로 만들어야 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_DIFFUSED_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VS_DIFFUSED_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_DIFFUSED_OUTPUT VSPlayer(VS_DIFFUSED_INPUT input)
{
	VS_DIFFUSED_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxPlayerWorld), gmtxView), gmtxProjection);
	output.color = input.color;

	return(output);
}

float4 PSPlayer(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
	return(input.color);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTexture : register(t0);
SamplerState gSamplerState : register(s0);
//SamplerState gClampSamplerState : register(s1);
struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_TEXTURED_OUTPUT VSTextured(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);

	/*VS_TEXTURED_OUTPUT output;

#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
	output.position = mul(mul(mul(float4(input.position, 1.0f), gcbGameObjectInfo.mtxWorld), gcbCameraInfo.mtxView), gcbCameraInfo.mtxProjection);
#else
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
#endif
	output.uv = input.uv;*/

	return(output);
}
float4 PSTextured(VS_TEXTURED_OUTPUT input, uint nPrimitiveID : SV_PrimitiveID) : SV_TARGET //픽셀쉐이더 
{


	float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);

	return(cColor);

}
VS_TEXTURED_OUTPUT VSInstancing(VS_TEXTURED_INPUT input, uint nInstanceID : SV_InstanceID)
{
	//esc 누르면 오류뜸
	VS_TEXTURED_OUTPUT output;
#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
	output.position = mul(mul(mul(float4(input.position, 1.0f), gGameObjectInfos[nInstanceID].m_mtxGameObject)), gmtxView), gmtxProjection);
#else
	output.position = mul(mul(mul(float4(input.position, 1.0f), gGameObjectInfos[nInstanceID].m_mtxGameObject), gmtxView), gmtxProjection);
#endif
	output.uv = input.uv;

	return(output);
}
//이거 추가

//StructuredBuffer<INSTANCEDGAMEOBJECTINFO> gGameObjectInfos : register(t0);
struct VS_INSTANCING_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;

};
struct VS_INSTANCING_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;

};
VS_INSTANCING_OUTPUT VSInstancing(VS_INSTANCING_INPUT input, uint nInstanceID :
SV_InstanceID)
{
	/*VS_INSTANCING_OUTPUT output;
	output.position = mul(mul(mul(float4(input.position, 1.0f),
		gGameObjectInfos[nInstanceID].m_mtxGameObject), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);*/
	VS_INSTANCING_OUTPUT output;
#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
	output.position = mul(mul(mul(float4(input.position, 1.0f), gGameObjectInfos[nInstanceID].m_mtxGameObject)), gmtxView), gmtxProjection);
#else
	output.position = mul(mul(mul(float4(input.position, 1.0f), gGameObjectInfos[nInstanceID].m_mtxGameObject), gmtxView), gmtxProjection);
#endif
	output.uv = input.uv;

	return(output);
}
//float4 PSInstancing(VS_INSTANCING_OUTPUT input) : SV_TARGET
float4 PSInstancing(VS_INSTANCING_OUTPUT input, uint nPrimitiveID : SV_PrimitiveID) : SV_TARGET
{
	float4 cColor;

	cColor = gtxtTexture.Sample(gSamplerState, input.uv);

	if (cColor.a < 0.1) discard;
	return(cColor);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTerrainBaseTexture : register(t1);
Texture2D gtxtTerrainDetailTexture : register(t2);
Texture2D gtxtSandTexture : register(t5);




struct VS_TERRAIN_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	//float4 color2 : COLOR1;
	float2 uv : TEXCOORD0;
	float2 detailuv : TEXCOORD1;

};
struct VS_TERRAIN_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	//float4 color2 : COLOR1;
	float2 uv : TEXCOORD0;
	float2 detailuv : TEXCOORD1;
};
VS_TERRAIN_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection); //찐
	output.color = input.color;
	output.uv = input.uv;
	output.detailuv = input.detailuv;

	return(output);
}

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
	float4 cBaseTexColor = float4(0.0f,0.0f,0.0f,0.0f);
	////float4 cDetailTexColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//
	////SOL1.
	cBaseTexColor = gtxtTerrainBaseTexture.Sample(gSamplerState, input.uv);
	float4 cDetailTexColor = gtxtTerrainDetailTexture.Sample(gSamplerState, input.detailuv);

	//SOL2.
	/*if (terrainID == 0)
	{
	cBaseTexColor = gtxtTerrainBaseTexture.Sample(gSamplerState, input.uv);
	}
	else 
	{
		cBaseTexColor = gtxtSandTexture.Sample(gSamplerState, input.uv);
	}
	float4 cDetailTexColor = gtxtTerrainDetailTexture.Sample(gSamplerState, input.detailuv);*/

	////cBaseTexColor = float4(terrainID, 0, 0, 1.0);

	////float4 cColor = cBaseTexColor;
	////float4 cColor;
	////float4 cColor = cBaseTexColor;
	////sol2
	float4 cColor = input.color * (cBaseTexColor+cDetailTexColor);
//	float4 cColor = input.color * saturate((cBaseTexColor * 0.1f) + (cDetailTexColor * 0.9f));

//float4 cColor = input.color * saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));

	//float4 cColor = cBaseTexColor;

	/*if (smoothstep(100,200, input.position.y) == 1)
		cColor = cBaseTexColor;
	else {
		cColor = cDetailTexColor;
	}*/
	/*if(input.position.x > 0)
		cColor = cBaseTexColor;
	else {
		cColor = cDetailTexColor;
	}*/

	return(cColor);
}

float4 PSSkyBox(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);

	return(cColor);
}


//VS_INSTANCING_OUTPUT VSInstancing(VS_INSTANCING_INPUT input, uint nInstanceID :
//SV_InstanceID)
//{
//	VS_INSTANCING_OUTPUT output;
//	output.position = mul(mul(mul(float4(input.position, 1.0f), gGameObjectInfos[nInstanceID].m_mtxGameObject), gmtxView), gmtxProjection);
//
//	output	gGameObjectInfos[nInstanceID].m_mtxGameObject), gmtxView), gmtxProjection);
//	//output.color = input.color + VS_VB_INSTANCE[nInstanceID].m_cColor;
//	return(output);
//}
//float4 PSInstancing(VS_INSTANCING_OUTPUT input) : SV_TARGET
//{
//	return(input.color);
//}