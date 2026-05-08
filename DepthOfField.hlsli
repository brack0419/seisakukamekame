struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
	float2 texcoord : TEXCOORD;
};

cbuffer CBDepthOfField : register(b0)
{
    float focusDistance; //	フォーカス距離
    float dofRange; //	焦点範囲
    float near_clip; // ニアクリップ面
    float far_clip; // ファークリップ面
};
