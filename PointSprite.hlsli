//--------------------------------------------
//	テクスチャ
//--------------------------------------------

Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

//--------------------------------------------
//	データーフォーマット
//--------------------------------------------

struct VSInput
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Size     : TEXCOORD;
	float4 Color    : COLOR;
	float4 Param    : PARAM;
};
struct GSInput
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Size     : TEXCOORD;
	float4 Color    : COLOR;
	float4 Param    : PARAM;
};

struct PSInput
{
	float4 Position : SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 Color : COLOR;
};

cbuffer CBPerFrame : register(b0)
{
	row_major matrix View; // ビュー変換行列
	row_major matrix Projection; // 透視変換行列
};
