#include "PBR.hlsli"

Texture2D albedoMap : register(t0);
Texture2D normalMap : register(t1); // 法線マップ
Texture2D metalnessSmoothnessMap : register(t2); // メタルネススムーズネスマップ
Texture2D occlusionMap : register(t3); // 遮蔽マップ
Texture2D shadowMap : register(t8); // シャドウマップ
TextureCube diffuseIrradianceEnvironmentMap : register(t17); //  拡散反射IBL用テクスチャ
TextureCube specularPremappingRadianceEnvironmentMap : register(t18); //  鏡面反射IBL用テクスチャ
Texture2D ggxLookUpTableMap : register(t19); //  GGXLUTテクスチャ

SamplerState baseMapSamplerState : register(s0);
SamplerState shadowMapSamplerState : register(s1);

//  ガンマ補正係数
static const float GammaFactor = 2.2f;

float4 main(VS_OUT pin) : SV_TARGET
{
	//	ベースカラーを取得
    float4 albedo = pin.color * materialColor;
    {
        float4 sampled = albedoMap.Sample(baseMapSamplerState, pin.texcoord);
        sampled.rgb = pow(sampled.rgb, GammaFactor);
        albedo *= sampled;
    }

	//	自己発光
    float3 emissive = float3(0, 0, 0);
    {
        //  テクスチャがないので無視
    }

    //  法線マッピング
    float3 N = normalMap.Sample(baseMapSamplerState, pin.texcoord).rgb;
    float3x3 mat = { normalize(pin.tangent), normalize(pin.binormal), normalize(pin.normal) };
    N = normalize(mul(N * 2.0f - 1.0f, mat));

    //  メタルネス/スムーズネステクスチャ
    float roughness, metalness;
    {
        float4 sampled = metalnessSmoothnessMap.Sample(baseMapSamplerState, pin.texcoord);
        roughness = 1.0f - sampled.a;   //  ラフネスに変換
        metalness = sampled.r;
    }

	//	光の遮蔽値を取得
    float occlusionFactor = 1.0f;
    const float occlusionStrength = 1.0f;
    {
        float4 sampled = occlusionMap.Sample(baseMapSamplerState, pin.texcoord);
        occlusionFactor = sampled.r;
    }

	//	入射光のうち拡散反射になる割合
    float3 diffuseReflectance = lerp(albedo.rgb, 0.0f, metalness);

	//	垂直反射時のフレネル反射率(非金属でも最低4%は鏡面反射する)
    float3 F0 = lerp(0.04f, albedo.rgb, metalness);

    //  視線ベクトル生成
    //float3 V = normalize(pin.world_position.xyz - viewPosition.xyz);
    float3 V = -normalize(viewPosition.xyz - pin.world_position.xyz);

    //  直接光のライティング
    float3 totalDiffuse = (float3) 0, totalSpecular = (float3) 0;

    //  平行光源
    {
        float3 diffuse = (float3) 0, specular = (float3) 0;
        float3 L = normalize(directionalLightData.direction.xyz);
        float3 LC = directionalLightData.color.rgb * directionalLightData.color.a;
        DirectBRDF(diffuseReflectance, F0, N, V, L, LC, roughness, diffuse, specular);

        //  シャドウマッピング
        float3 shadow = CalcShadowColorPCFFilter(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor, shadowBias, PCFKernelSize);
        totalDiffuse += diffuse * shadow;
        totalSpecular += specular * shadow;
    }
    
    //  ポイントライト
    for (int i = 0; i < pointLightCount; ++i)
    {
		// ライトベクトルを算出
        float3 lightVector = pin.world_position.xyz - pointLightData[i].position.xyz;

		// ライトベクトルの長さを算出
        float lightLength = length(lightVector);

		// ライトの影響範囲外なら後の計算をしない。
        if (lightLength >= pointLightData[i].range)
            continue;

		// 距離減衰を算出する
        float attenuate = saturate(1.0f - lightLength / pointLightData[i].range);
        attenuate *= pointLightData[i].influence;
        lightVector = lightVector / lightLength;

        // シェーディング
        float3 diffuse = (float3) 0, specular = (float3) 0;
        float3 L = normalize(directionalLightData.direction.xyz);
        float3 LC = pointLightData[i].color.rgb * pointLightData[i].color.a;
        DirectBRDF(diffuseReflectance, F0, N, V, lightVector, LC, roughness, diffuse, specular);
        totalDiffuse += diffuse * attenuate;
        totalSpecular += specular * attenuate;
    }

    //  スポットライト
    for (int j = 0; j < spotLightCount; ++j)
    {
		// ライトベクトルを算出
        float3 lightVector = pin.world_position.xyz - spotLightData[j].position.xyz;

		// ライトベクトルの長さを算出
        float lightLength = length(lightVector);

        if (lightLength >= spotLightData[j].range)
            continue;

		// 距離減衰を算出する
        float attenuate = saturate(1.0f - lightLength / spotLightData[j].range);
        attenuate *= spotLightData[j].influence;
        lightVector = normalize(lightVector);

		// 角度減衰を算出してattenuateに乗算する
        float3 spotDirection = normalize(spotLightData[j].direction.xyz);
        float angle = dot(spotDirection, lightVector);
        float area = spotLightData[j].innerCorn - spotLightData[j].outerCorn;
        attenuate *= saturate(1.0f - (spotLightData[j].innerCorn - angle) / area);

        // シェーディング
        float3 diffuse = (float3) 0, specular = (float3) 0;
        float3 L = normalize(directionalLightData.direction.xyz);
        float3 LC = spotLightData[j].color.rgb * spotLightData[j].color.a;
        DirectBRDF(diffuseReflectance, F0, N, V, lightVector, LC, roughness, diffuse, specular);
        totalDiffuse += diffuse * attenuate;
        totalSpecular += specular * attenuate;
    }

    //  イメージベースドライティング
    {
        float3 ambient = ambientLightColor.rgb * ambientLightColor.a;
        totalDiffuse += ambient * DiffuseIBL(N, V, roughness, diffuseReflectance, F0,
                                diffuseIrradianceEnvironmentMap, baseMapSamplerState);
        totalSpecular += ambient * SpecularIBL(N, V, roughness, F0,
                                ggxLookUpTableMap, specularPremappingRadianceEnvironmentMap, baseMapSamplerState);
    }

	//	遮蔽処理
    totalDiffuse = lerp(totalDiffuse, totalDiffuse * occlusionFactor, occlusionStrength);
    totalSpecular = lerp(totalSpecular, totalSpecular * occlusionFactor, occlusionStrength);

	//	色生成
    float3 color = totalDiffuse + totalSpecular + emissive;
    color.rgb = pow(color.rgb, 1.0f / GammaFactor); //  sRGB空間へ
    return float4(color, albedo.a);
}
