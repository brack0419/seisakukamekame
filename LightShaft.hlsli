#include "Light.hlsli"

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer CBLightShaft : register(b0)
{
    int iteration; // レイマーチングの回数
    float intensity; //  明るさの強さ
    float adjustLength; // 初期距離調整
    uint dummy1; // 黒色にライトシャフトをかけるフラグ

    float4 viewPosition;

    row_major float4x4 inverseViewProjection;

	// 平行光源情報処理
    DirectionalLightData directionalLightData;

    // シャドウマップ情報
    row_major float4x4 lightViewProjection; // ライトビュープロジェクション行列
    float3 shadowColor; // 影の色
    float shadowBias; // 深度比較用のオフセット値
    int PCFKernelSize; // ソフトシャドーのサイズ
    float3 dummy2;
};
