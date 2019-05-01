
VS_TEXTURED_OUTPUT VSTexturedUI(VS_TEXTURED_INPUT input, uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;

	if (nVertexID == 0)
	{
		output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);

	}
	if (nVertexID == 1)
	{
		output.position = float4(+1.0f, +1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 0.0f);

	}
	if (nVertexID == 2)
	{
		output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);

	}
	if (nVertexID == 3)
	{
		output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);

	}
	if (nVertexID == 4)
	{
		output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);

	}
	if (nVertexID == 5)
	{
		output.position = float4(-1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 1.0f);

	}
	return (output);

}

float4 PSTexturedUI(VS_TEXTURED_OUTPUT input) :SV_Target
{



	float4 cColor = gtxtPilot.Sample(gSamplerState, input.uv);




	return(cColor);

}

VS_TEXTURED_OUTPUT VSTexturedMenuUI(VS_TEXTURED_INPUT input, uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;

	if (nVertexID == 0)
	{
		output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);

	}
	if (nVertexID == 1)
	{
		output.position = float4(+1.0f, +1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 0.0f);

	}
	if (nVertexID == 2)
	{
		output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);

	}
	if (nVertexID == 3)
	{
		output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);

	}
	if (nVertexID == 4)
	{
		output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);

	}
	if (nVertexID == 5)
	{
		output.position = float4(-1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 1.0f);

	}
	return (output);

}

Texture2D gtxtGameMode : register(t23);
float4 PSTexturedMenuUI(VS_TEXTURED_OUTPUT input) :SV_Target
{

	float4 cColor = gtxtGameMode.Sample(gSamplerState, input.uv);


	return(cColor);

}
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
float4 PSTextured(VS_TEXTURED_OUTPUT input, uint nPrimitiveID : SV_PrimitiveID) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
{


	float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);

	return(cColor);

}
VS_TEXTURED_OUTPUT VSInstancing(VS_TEXTURED_INPUT input, uint nInstanceID : SV_InstanceID)
{
	//esc ´©¸£¸é ¿À·ù¶ä
	VS_TEXTURED_OUTPUT output;
#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
	output.position = mul(mul(mul(float4(input.position, 1.0f), gGameObjectInfos[nInstanceID].m_mtxGameObject)), gmtxView), gmtxProjection);
#else
	output.position = mul(mul(mul(float4(input.position, 1.0f), gGameObjectInfos[nInstanceID].m_mtxGameObject), gmtxView), gmtxProjection);
#endif
	output.uv = input.uv;

	return(output);
}
//ÀÌ°Å Ãß°¡

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