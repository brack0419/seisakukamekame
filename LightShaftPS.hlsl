#include "LightShaft.hlsli"

Texture2D depth_map : register(t0);
Texture2D shadow_map : register(t1);

SamplerState depth_sampler_state : register(s0);
SamplerState shadow_map_sampler_states : register(s1);

//  ランダム関数
float rand(float2 texel)
{
    return frac(sin(dot(texel, float2(12.9898f, 78.233f))) * 43758.5453f);
}
float4 main(VS_OUT pin) : SV_TARGET
{
    if (intensity < 0.01f)
        return 0;

    // レイマーチングを行うためレイの終点座標を求める。
    float3 world_position, ray_limit_position;
    {
        //  シーンの深度を取得
        float depth = depth_map.Sample(depth_sampler_state, pin.texcoord);
        //  シーンのNDC座標を求めて、そこからワールド座標に逆変換
        float4 ndc = float4(pin.texcoord.xy * float2(2, -2) + float2(-1, +1), depth, 1);
        float4 position = mul(ndc, inverseViewProjection);
        world_position = position.xyz / position.w;
        ray_limit_position = world_position;
    }

    //  カメラから終端へのベクトルを求める
    float3 ray_dir = world_position - viewPosition.xyz;
    float3 ray_step = ray_dir / iteration;

    // カメラからレイの終端に向けて進めて行く
    float3 ray_position = viewPosition.xyz;
    ray_position += ray_step * (rand(pin.texcoord.xy) * adjustLength); // 始点

    //  レイマーチングを行う
    float step_intensity = 1.0f / iteration;
    float shaft = 0;
    [loop] for (int i = 1; i < iteration; ++i)
    {
        //  シャドウマップ用のパラメーター計算
        float3 shadow_texcoord;
        {
            //  ライトから見たNDC座標を算出
            float4 pos = mul(float4(ray_position, 1.0f), lightViewProjection);
            pos /= pos.w;
            pos.y = -pos.y;
            pos.xy = pos.xy * 0.5f + 0.5f;
            shadow_texcoord = pos.xyz;
        }

        // 平行光源用シャドウマップ
        float depth = shadow_map.Sample(shadow_map_sampler_states, shadow_texcoord.xy).r;

        //  影じゃなければ光が当たってるので加算
        shaft += step_intensity * step(shadow_texcoord.z - depth, shadowBias);

        //  レイを進める
        ray_position += ray_step * (rand(ray_position.xy + ray_position.zz) * 0.5f + 0.5f);
    }

    shaft = smoothstep(0.0f, 2.0f, shaft * shaft * shaft); //  強めに係るようにする
    return float4(directionalLightData.color.rgb * directionalLightData.color.a * shaft * intensity, 1);
}
