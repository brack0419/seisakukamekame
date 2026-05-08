#include "PointSprite.hlsli"
//--------------------------------------------
//	ジオメトリシェーダー
//--------------------------------------------
// 1頂点 → ４角形
[maxvertexcount(4)]
void main(point GSInput In[1],
	inout TriangleStream<PSInput> ParticleStream)
{
	// 座標変換 (ワールド座標系 → ビュー座標系)
	float4 pos = mul(float4(In[0].Position, 1.0),View);
	// 点を面にする(４頂点を作る)
	float rot = In[0].Param.x;
	float s = sin(rot);
	float c = cos(rot);
	float4 right =
		float4(c,-s,0,0) * (In[0].Size.x*0.5);
	float4 up =
		float4(s, c,0,0) * (In[0].Size.y*0.5);

	float4 pos_left_top = pos - right + up;
	float4 pos_left_bottom = pos - right - up;
	float4 pos_right_top = pos + right + up;
	float4 pos_right_bottom = pos + right - up;

	// タイプ設定
	uint type = (uint)In[0].Param.y;
	uint komax = (uint)In[0].Param.z;
	uint komay = (uint)In[0].Param.w;
	float w = 1.0 / komax;
	float h = 1.0 / komay;
	float2 uv = float2((type%komax)*w,(type/komax)*h );

	// 左上の点の位置(射影座標系)・UV・色を計算して出力
	PSInput Out = (PSInput)0;
	Out.Color = In[0].Color;
	Out.Position = mul(pos_left_top, Projection);
	Out.Tex = uv + float2(0, 0); // テクスチャ左上
	ParticleStream.Append(Out);
	// 右上の点の位置(射影座標系) とテクスチャ座標の計算をして出力
	Out.Color = In[0].Color;
	Out.Position = mul(pos_right_top, Projection);
	Out.Tex = uv + float2(w, 0); // テクスチャ
	ParticleStream.Append(Out);

	// 左下の点の位置(射影座標系) とテクスチャ座標の計算をして出力
	Out.Color = In[0].Color;
	Out.Position = mul(pos_left_bottom, Projection);
	Out.Tex = uv + float2(0, h); // テクスチャ
	ParticleStream.Append(Out);

	// 右下の点の位置(射影座標系) とテクスチャ座標の計算をして出力
	Out.Color = In[0].Color;
	Out.Position = mul(pos_right_bottom, Projection);
	Out.Tex = uv + float2(w, h); // テクスチャ
	ParticleStream.Append(Out);

	ParticleStream.RestartStrip();
}