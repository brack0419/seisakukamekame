#include "LuminanceExtraction.hlsli"

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float RGB2Luminance(float3 rgb)
{
    return dot(rgb, float3(0.299f, 0.587f, 0.114f));
}

float4 main(VS_OUT pin) : SV_TARGET
{
	float4 color = texture0.Sample(sampler0, pin.texcoord) * pin.color;

	// RGB > 輝度値に変換
	float luminance = RGB2Luminance(color.rgb);

	// 閾値との差を算出
	float contribution = max(0, luminance - threshold);

	// 出力する色を補正する
	contribution /= luminance;
	color.rgb *= contribution * intensity;
	return color;
}
