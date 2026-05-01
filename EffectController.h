#pragma once
#include "Effect.h"
#include <DirectXMath.h>

class EffectController
{
public:
	EffectController();
	~EffectController();

	void Update(float elapsedTime);
	void DrawDebugGUI();

	void PlayAura();
	void SetAuraPosition(const DirectX::XMFLOAT3& pos);

private:
	struct AuraEffectData
	{
		Effect* effect = nullptr;

		float scale = 0.4f;
		float alpha = 0.5f;

		bool autoPreview = false;
		float previewTimer = 0.0f;
		float previewInterval = 1.0f;

		DirectX::XMFLOAT3 worldPosition = { 0,1,0 };
	};

	AuraEffectData auraFX;
};