#include "DepthOfField.hlsli"

Texture2D color_map : register(t0);
Texture2D bokeh_map : register(t1);
Texture2D depth_map : register(t2);

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3] : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
    //  深度値を取得
    float depth = depth_map.Sample(sampler_states[POINT], pin.texcoord);

    //  深度値から距離を求める
    //  float view_space_z = near * far / (far - depth * (far - near));
    float view_space_z = (near_clip * far_clip) / (far_clip - depth * (far_clip - near_clip));

    //  焦点距離と焦点範囲からブレンド係数を算出
    float alpha = abs(view_space_z - focusDistance) / dofRange;
    alpha = saturate(alpha);

    //  ぼかし色取得
    float3 origin_color = color_map.Sample(sampler_states[LINEAR], pin.texcoord).rgb;
    float3 bokeh_color = bokeh_map.Sample(sampler_states[LINEAR], pin.texcoord).rgb;
    float3 color = bokeh_color * (alpha) + origin_color.rgb * (1.0 - alpha);
    return float4(color, 1);
}
