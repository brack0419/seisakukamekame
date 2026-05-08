//pointsprite_vs.hlsl
#include "PointSprite.hlsli"
//--------------------------------------------
//	頂点シェーダー
//--------------------------------------------

GSInput main(VSInput input)
{
	GSInput output = (GSInput)0;
	output.Position = input.Position;
	output.Color = input.Color;
	output.Size = input.Size;
	output.Normal = input.Normal;
	
	output.Param = input.Param;
	return output;
}

