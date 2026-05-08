//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

#include "UVScroll.hlsli"

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
    //return float4(1, 0, 0, 1);
    return texture0.Sample(sampler0, pin.texcoord) * pin.color;
}
