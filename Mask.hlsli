struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

//	スプライト描画単位で指定するパラメーター
cbuffer CbScene : register(b0)
{
    float2 uvScrollValue; // UVスクロール値
    float2 dummy1; // ダミー
};

////	スプライト描画単位で指定するパラメーター
//cbuffer CbMask : register(b1) // 1番にある定数バッファをこれで利用する
//{
//    float dissolveThreshold; // ディゾルブ量
//    float3 dummy2;
//};

//	スプライト描画単位で指定するパラメーター
cbuffer CbMask : register(b1) // 0番にある定数バッファをこれで利用する
{
    float dissolveThreshold; // ディゾルブ量
    float edgeThreshold; // 縁の閾値
    float2 dummy2;
    float4 edgeColor; // 縁の色
};
