//
Texture2D gtxtTexture : register(t14);

#include "Shaders.hlsl"
Texture2D gtxtUITexture : register(t15);
Texture2D gtxtSkillUITexture : register(t16);

SamplerState gSamplerState : register(s0);
cbuffer cbUIInfo : register(b5)
{
	float3 position : POSITION: packoffset(c0);
	float2 uv : TEXCOORD: packoffset(c3);
};
cbuffer cbHpInfo : register(b9)
{
	int hpRatio : packoffset(c0);
};

cbuffer cboneClock : register(b11)
{
	int oneSec : packoffset(c0);

};
cbuffer cbTenClock : register(b12)
{
	int teneSec : packoffset(c0);
};
cbuffer cbHunClock : register(b13)
{
	int hunSec : packoffset(c0);
};

cbuffer cbSkillCoolDownInfo : register(b10)
{
	int coolDown : packoffset(c0);
};
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


	return(output);
}
//ÀÌ°Å Ãß°¡

//StructuredBuffer<INSTANCEDGAMEOBJECTINFO> gGameObjectInfos : register(t0);
//struct VS_INSTANCING_INPUT
//{
//	float3 position : POSITION;
//	float2 uv : TEXCOORD;
//
//};
//struct VS_INSTANCING_OUTPUT
//{
//	float4 position : SV_POSITION;
//	float2 uv : TEXCOORD;
//
//};
//VS_INSTANCING_OUTPUT VSInstancing(VS_INSTANCING_INPUT input, uint nInstanceID :
//SV_InstanceID)
//{
//	/*VS_INSTANCING_OUTPUT output;
//	output.position = mul(mul(mul(float4(input.position, 1.0f),
//		gGameObjectInfos[nInstanceID].m_mtxGameObject), gmtxView), gmtxProjection);
//	output.uv = input.uv;
//
//	return(output);*/
//	VS_INSTANCING_OUTPUT output;
//#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
//	output.position = mul(mul(mul(float4(input.position, 1.0f), gGameObjectInfos[nInstanceID].m_mtxGameObject)), gmtxView), gmtxProjection);
//#else
//	output.position = mul(mul(mul(float4(input.position, 1.0f), gGameObjectInfos[nInstanceID].m_mtxGameObject), gmtxView), gmtxProjection);
//#endif
//	output.uv = input.uv;
//
//	return(output);
//}
////float4 PSInstancing(VS_INSTANCING_OUTPUT input) : SV_TARGET
//float4 PSInstancing(VS_INSTANCING_OUTPUT input, uint nPrimitiveID : SV_PrimitiveID) : SV_TARGET
//{
//	float4 cColor;
//
//	cColor = gtxtTexture.Sample(gSamplerState, input.uv);
//
//	if (cColor.a < 0.1) discard;
//	return(cColor);
//}

VS_TEXTURED_OUTPUT VSUITextured(uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;
	//0.5Á¤»ç°¢Çü ÁÂÃø»ó´Ü
	/*if (nVertexID == 0) { output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(-0.5f, +1.0f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(-1.0f, +0.5f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }
	*/
	//È­¸é ÀüÃ¼
	if (nVertexID == 0) { output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(+1.0f, +1.0f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(-1.0f, -1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }

	return output;

}
float4 PSUITextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
{

	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);

	return(cColor);

}
VS_TEXTURED_OUTPUT VSPlayerUITextured(uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;
	//0.5Á¤»ç°¢Çü ÁÂÃø»ó´Ü
	/*if (nVertexID == 0) { output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(-0.5f, +1.0f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(-1.0f, +0.5f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }
	*/
	//È­¸é ÀüÃ¼
	if (nVertexID == 0) { output.position = float4(-0.9f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(-0.7f, +1.0f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(-0.7f, 0.7f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(-0.9f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(-0.7f, +0.7f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(-0.9f, +0.7f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }

	return output;

}
float4 PSPlayerUITextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
{

	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);

	if (cColor.a < 0.1) discard;
	return(cColor);

}

VS_TEXTURED_OUTPUT VSOtherPlayerUITextured(uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;
	//0.5Á¤»ç°¢Çü ÁÂÃø»ó´Ü
	/*if (nVertexID == 0) { output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(-0.5f, +1.0f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(-1.0f, +0.5f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }
	*/
	//È­¸é ÀüÃ¼
	if (nVertexID == 0) { output.position = float4(+0.7f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(+0.9f, +1.0f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(+0.9f, 0.7f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(+0.7f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(+0.9f, +0.7f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(+0.7f, 0.7f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }

	return output;

}
float4 PSOtherPlayerUITextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
{

	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);

	if (cColor.a < 0.1) discard;
	return(cColor);

}
VS_TEXTURED_OUTPUT VSSkillUITextured(uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;
	//0.5Á¤»ç°¢Çü ÁÂÃø»ó´Ü
	/*if (nVertexID == 0) { output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(-0.5f, +1.0f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(-1.0f, +0.5f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }
	*/
	//È­¸é ÀüÃ¼
	if (nVertexID == 0) { output.position = float4(-1.0f, -0.8f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(-0.8f, -0.8f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(-0.8f, -1.0f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(-1.0f, -0.8f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(-0.8f, -1.0f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(-1.0f, -1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }

	return output;

}
float4 PSSSkillTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
{

	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);

	if (cColor.a < 0.1) discard;
	return(cColor);

}
VS_TEXTURED_OUTPUT VSTimeBarTextured(uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;
	//0.5Á¤»ç°¢Çü ÁÂÃø»ó´Ü
	/*if (nVertexID == 0) { output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(-0.5f, +1.0f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(-1.0f, +0.5f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }
	*/
	//È­¸é ÀüÃ¼
	if (nVertexID == 0) { output.position = float4(-0.3f, +0.75f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(+0.3f, +0.75f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(+0.3f, +0.65f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(-0.3f, +0.75f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(+0.3f, +0.65f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(-0.3f, +0.65f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }

	return output;

}
float4 PSSTimeBarTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
{

	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);

	if (cColor.a < 0.1) discard;
	return(cColor);

}
VS_TEXTURED_OUTPUT VSWinTextured(uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;

	if (nVertexID == 0) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(+0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(+0.5f, -0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(+0.5f, -0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(-0.5f, -0.5f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }
	return output;

}
float4 PSWinTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
{

	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);

	if (cColor.a < 0.1) discard;
	return(cColor);

}


////effect
VS_TEXTURED_OUTPUT VSEffectTextured(VS_TEXTURED_INPUT input, uint nVertexID : SV_VertexID)
{


	VS_TEXTURED_OUTPUT output;
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return (output);
}
float4 PSEffectTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
{

	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);

	//if (cColor.a < 0.1) discard;
	return(cColor);

}


VS_TEXTURED_OUTPUT VSLoseTextured(uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;

	if (nVertexID == 0) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(+0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(+0.5f, -0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(-0.5f, +0.5f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(+0.5f, -0.5f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(-0.5f, -0.5f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }
	return output;

}
float4 PSLoseTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
{

	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);

	if (cColor.a < 0.1) discard;
	return(cColor);

}

//HP
VS_TEXTURED_OUTPUT VSHPTextured(uint nVertexID : SV_VertexID)
//VS_TEXTURED_OUTPUT VSHPTextured(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

	//output.position = float4(input.position.x * hpRatio + (0.5f * hpRatio) - 1.f, input.position.y - 0.75f, input.position.z, 1.0f);
	//output.uv = input.uv;

	if (nVertexID == 0) { output.position = float4(0.2f, -0.55f, 0.0f, 1.0f); output.uv = float2(0.125f * hpRatio, 0.f); }
	if (nVertexID == 1) { output.position = float4(0.5f, -0.55f, 0.0f, 1.0f); output.uv = float2(0.125 + (0.125f * hpRatio), 0.f); }
	if (nVertexID == 2) { output.position = float4(0.5f, -0.95f, 0.0f, 1.0f); output.uv = float2(0.125 + (0.125f * hpRatio), 1.f); }
	if (nVertexID == 3) { output.position = float4(0.2f, -0.55f, 0.0f, 1.0f); output.uv = float2(0.125f * hpRatio, 0.f); }
	if (nVertexID == 4) { output.position = float4(0.5f, -0.95f, 0.0f, 1.0f); output.uv = float2(0.125 + (0.125f * hpRatio), 1.f); }
	if (nVertexID == 5) { output.position = float4(0.2f, -0.95f, 0.0f, 1.0f); output.uv = float2(0.125 * hpRatio, 1.f); }


	//output.position = float4(0.7, -0.2, input.position.z, 1.0f);
	//output.uv = input.uv;

	return output;

}
float4 PSHPTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
{
	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);

	if (cColor.a < 0.1) discard;
	return(cColor);

}

//chat

VS_TEXTURED_OUTPUT VSChatTextured(uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;

	if (nVertexID == 0) { output.position = float4(-1.0f,  0.f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(-0.6f,  0.0f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(-0.6f, 0.0f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(-1.0f,  0.0f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(-0.6f, 0.0f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(-1.0f,  0.0f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }
	return output;

}
float4 PSChatTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
{

	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);

	//cColor.a = 0.01;
	if (cColor.a < 0.1) discard;
	return(cColor);

}

VS_TEXTURED_OUTPUT VSSkillCoolTextured(uint nVertexID : SV_VertexID)
{
	//VS_TEXTURED_OUTPUT output;




	//if (nVertexID == 0) { output.position = float4(-0.95f, -0.75f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f);}
	//if (nVertexID == 1) { output.position = float4(-0.75f, -0.75f, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	//if (nVertexID == 2) { output.position = float4(-0.75f, -0.95f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	//if (nVertexID == 3) { output.position = float4(-0.95f, -0.75f, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	//if (nVertexID == 4) { output.position = float4(-0.75f, -0.95f, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	//if (nVertexID == 5) { output.position = float4(-0.95f, -0.95f, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }
	//return output;

	VS_TEXTURED_OUTPUT output;

	if (nVertexID == 0) { output.position = float4(0.65f, -0.65f, 0.0f, 1.0f); output.uv = float2(0.25f * coolDown, 0.f);}
	if (nVertexID == 1) { output.position = float4(0.95f, -0.65f, 0.0f, 1.0f); output.uv = float2(0.25 + (0.25f * coolDown), 0.f); }
	if (nVertexID == 2) { output.position = float4(0.95f, -0.95f, 0.0f, 1.0f); output.uv = float2(0.25 + (0.25f * coolDown), 1.f); }
	if (nVertexID == 3) { output.position = float4(0.65f, -0.65f, 0.0f, 1.0f); output.uv = float2(0.25f * coolDown, 0.f); }
	if (nVertexID == 4) { output.position = float4(0.95f, -0.95f, 0.0f, 1.0f); output.uv = float2(0.25 + (0.25f * coolDown), 1.f); }
	if (nVertexID == 5) { output.position = float4(0.65f, -0.95f, 0.0f, 1.0f); output.uv = float2(0.25 * coolDown, 1.f); }
	return output;
}
float4 PSSkillCoolTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
{

	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);

	if (cColor.a < 0.1) discard;
	return(cColor);

}

VS_TEXTURED_OUTPUT VSOneSecTextured(uint nVertexID : SV_VertexID)
{
	//VS_TEXTURED_OUTPUT output;

	VS_TEXTURED_OUTPUT output;
	if (nVertexID == 0) { output.position = float4(0.1f, 0.95f, 0.0f, 1.0f); output.uv = float2(0.1f * oneSec, 0.f);}
	if (nVertexID == 1) { output.position = float4(0.3f, 0.95f, 0.0f, 1.0f); output.uv = float2(0.1 + (0.1f * oneSec), 0.f); }
	if (nVertexID == 2) { output.position = float4(0.3f, 0.75f, 0.0f, 1.0f); output.uv = float2(0.1 + (0.1f * oneSec), 1.f); }
	if (nVertexID == 3) { output.position = float4(0.1f, 0.95f, 0.0f, 1.0f); output.uv = float2(0.1f * oneSec, 0.f); }
	if (nVertexID == 4) { output.position = float4(0.3f, 0.75f, 0.0f, 1.0f); output.uv = float2(0.1 + (0.1f * oneSec), 1.f); }
	if (nVertexID == 5) { output.position = float4(0.1f, 0.75f, 0.0f, 1.0f); output.uv = float2(0.1 * oneSec, 1.f); }
	return output;
}
VS_TEXTURED_OUTPUT VSTenSecTextured(uint nVertexID : SV_VertexID)
{
	//VS_TEXTURED_OUTPUT output;

	VS_TEXTURED_OUTPUT output;
	if (nVertexID == 0) { output.position = float4(-0.1f, 0.95f, 0.0f, 1.0f); output.uv = float2(0.1f * teneSec, 0.f);}
	if (nVertexID == 1) { output.position = float4(0.1f, 0.95f, 0.0f, 1.0f); output.uv = float2(0.1 + (0.1f * teneSec), 0.f); }
	if (nVertexID == 2) { output.position = float4(0.1f, 0.75f, 0.0f, 1.0f); output.uv = float2(0.1 + (0.1f * teneSec), 1.f); }
	if (nVertexID == 3) { output.position = float4(-0.1f, 0.95f, 0.0f, 1.0f); output.uv = float2(0.1f * teneSec, 0.f); }
	if (nVertexID == 4) { output.position = float4(0.1f, 0.75f, 0.0f, 1.0f); output.uv = float2(0.1 + (0.1f * teneSec), 1.f); }
	if (nVertexID == 5) { output.position = float4(-0.1f, 0.75f, 0.0f, 1.0f); output.uv = float2(0.1 * teneSec, 1.f); }
	return output;
}
VS_TEXTURED_OUTPUT VSHundredSecTextured(uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;
	if (nVertexID == 0) { output.position = float4(-0.3f, 0.95f, 0.0f, 1.0f); output.uv = float2(0.1f * hunSec, 0.f); }
	if (nVertexID == 1) { output.position = float4(-0.1f, 0.95f, 0.0f, 1.0f); output.uv = float2(0.1 + (0.1f * hunSec), 0.f); }
	if (nVertexID == 2) { output.position = float4(-0.1f, 0.75f, 0.0f, 1.0f); output.uv = float2(0.1 + (0.1f * hunSec), 1.f); }
	if (nVertexID == 3) { output.position = float4(-0.3f, 0.95f, 0.0f, 1.0f); output.uv = float2(0.1f * hunSec, 0.f); }
	if (nVertexID == 4) { output.position = float4(-0.1f, 0.75f, 0.0f, 1.0f); output.uv = float2(0.1 + (0.1f * hunSec), 1.f); }
	if (nVertexID == 5) { output.position = float4(-0.3f, 0.75f, 0.0f, 1.0f); output.uv = float2(0.1 * hunSec, 1.f); }
	return output;
}
float4 PSClockTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
{

	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);

	if (cColor.a < 0.1) discard;
	return(cColor);

}
//
//float4 PSOneSecTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
//{
//
//	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);
//
//	if (cColor.a < 0.1) discard;
//	return(cColor);
//
//}
//float4 PSTenSceTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET //ÇÈ¼¿½¦ÀÌ´õ 
//{
//
//	float4 cColor = gtxtUITexture.Sample(gSamplerState, input.uv);
//
//	if (cColor.a < 0.1) discard;
//	return(cColor);
//
//}