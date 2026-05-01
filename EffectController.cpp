#include <imgui.h>
#include "EffectController.h"

EffectController::EffectController()
{
	auraFX.effect = new Effect("Data/Effect/Aura.efk");
}

EffectController::~EffectController()
{
	delete auraFX.effect;
}

void EffectController::SetAuraPosition(const DirectX::XMFLOAT3& pos)
{
	auraFX.worldPosition = pos;
}

void EffectController::PlayAura()
{
	auto handle = auraFX.effect->Play(auraFX.worldPosition);

	DirectX::XMFLOAT3 scaleVec =
	{
		auraFX.scale,
		auraFX.scale,
		auraFX.scale
	};

	auraFX.effect->SetScale(handle, scaleVec);

	DirectX::XMFLOAT4 color =
	{
		1.0f,
		1.0f,
		1.0f,
		auraFX.alpha
	};

	auraFX.effect->SetColor(handle, color);
}

void EffectController::Update(float elapsedTime)
{
	if (auraFX.autoPreview)
	{
		auraFX.previewTimer += elapsedTime;

		if (auraFX.previewTimer >= auraFX.previewInterval)
		{
			auraFX.previewTimer = 0.0f;
			PlayAura();
		}
	}
}

void EffectController::DrawDebugGUI()
{
	if (ImGui::Begin("Effect_Aura"))
	{
		ImGui::Checkbox("Auto Aura Preview", &auraFX.autoPreview);

		ImGui::SliderFloat(
			"Aura Interval",
			&auraFX.previewInterval,
			0.1f,
			3.0f
		);

		ImGui::SliderFloat3(
			"Aura World Pos",
			&auraFX.worldPosition.x,
			-50.0f,
			50.0f
		);

		ImGui::SliderFloat(
			"Aura Scale",
			&auraFX.scale,
			0.001f,
			1.0f
		);

		ImGui::SliderFloat(
			"Aura Alpha",
			&auraFX.alpha,
			0.0f,
			1.0f
		);

		if (ImGui::Button("Play Aura"))
		{
			PlayAura();
		}
	}
	ImGui::End();
}