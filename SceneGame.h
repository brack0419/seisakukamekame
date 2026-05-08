#pragma once

//==================================================
// Scene / Core
//==================================================
#include "Scene.h"
#include "Stage.h"
#include "Player.h"
#include "CameraController.h"

//==================================================
// Graphics Core
//==================================================
#include "Graphics/RenderTarget.h"
#include "Graphics/DepthStencil.h"
#include "Graphics/SkyBox.h"
#include "Graphics/Texture.h"

//==================================================
// Lighting
//==================================================
#include "Light.h"

//==================================================
// Effects / Particles
//==================================================
#include "ParticleSystem.h"
#include "EffectController.h"
#include <Turret.h>

//==================================================
// Game Scene
//==================================================
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// èâä˙âª
	void Initialize() override;

	// èIóπèàóù
	void Finalize() override;

	// çXêVèàóù
	void Update(float elapsedTime) override;

	// ï`âÊèàóù
	void Render() override;

private:

	//==================================================
	// Render Pipeline
	//==================================================
	void Render3DScene();
	void RenderShadowmap();
	void RenderPostprocess();

	void ClearRenderTargetSlots();
	void ClearConstantBuffers(int start_slot = 0, int num = D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	void ClearShaderResorceViews(int start_slot = 0, int num = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	void ClearUnorderedAccessViews(int start_slot = 0, int num = 8);
	void ClearSampler(int start_slot = 0, int num = D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);

private:

	//==================================================
	// Scene Objects
	//==================================================
	Stage* stage = nullptr;
	Player* player = nullptr;
	CameraController* cameraController = nullptr;
	SkyBox* skybox = nullptr;
	Turret* turret = nullptr;
	//==================================================
	// Lighting
	//==================================================
	Light* mainDirectionalLight = nullptr;

	DirectX::XMFLOAT3 shadowColor = { 0.2f, 0.2f, 0.2f };
	DirectX::XMFLOAT4X4 lightViewProjection;

	float shadowDrawRect = 150.0f;
	float shadowBias = 0.00015f;

	int PCFKernelSize = 15;

	//==================================================
	// Shadow Map
	//==================================================
	std::unique_ptr<DepthStencil> shadowmapDepthStencil;

	//==================================================
	// Post Processing
	//==================================================
	std::unique_ptr<RenderTarget> offscreen;
	std::unique_ptr<RenderTarget> workRenderTargets[2];
	std::unique_ptr<DepthStencil> sceneDepthStencil;

	std::unique_ptr<Sprite> screen_sprite;

	int sceneBlurSize = 0;
	int bloomBlurSize = 10;

	int depthOfFieldBlurSize = 5;
	float focusDistance = 15.0f;
	float dofRange = 20.0f;

	LuminanceExtractionData luminanceExtractionData;

	//==================================================
	// IBL
	//==================================================
	std::unique_ptr<Texture> iblDiffuseTexture;
	std::unique_ptr<Texture> iblSpecularTexture;
	std::unique_ptr<Texture> iblGGXLutTexture;

	//==================================================
	// Particles / Effects
	//==================================================
	std::unique_ptr<cParticleSystem> bomb_particle;
	std::unique_ptr<Texture> bomb_texture;

	std::unique_ptr<cParticleSystem> particle;
	std::unique_ptr<Texture> texture;

	EffectController effectController;

	//==================================================
	// Light Shaft
	//==================================================
	int lightShaftIteration = 24;
	float lightShaftIntensity = 0.5f;
	float lightShaftAdjustLength = 10.0f;

	//==================================================
	// Misc (Visual Debug / Effects)
	//==================================================
	float outlinewidth = 0.0f;
	DirectX::XMFLOAT4 outlineColor{ 0,0,0,1 };
};