#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"
#include "Effect.h"
#include <DirectXMath.h>
#include "EffectController.h"

#define PLAYER_HP 100

// プレイヤー
class Player : public Character
{
public:
	Player();
	~Player() override;

	// インスタンス取得
	static Player& Instance();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(const RenderContext& rc, ModelShader* shader);

	// デバッグGUI
	void DrawDebugGUI();

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// 前方取得
	DirectX::XMFLOAT3 GetDirection() const { return direction; }

protected:
	// 着地時
	void OnLanding() override;

private:
	// ========= 内部処理 =========
	DirectX::XMFLOAT3 GetMoveVec() const;

	void InputMove(float elapsedTime);
	void InputJump();
	void InputProjectile();

	void CollisionPlayerVsEnemies();
	void CollisionProjectilesVsEnemies();

	void StartAppearance();

	// ========= アニメーション =========
	enum Animation
	{
		Anim_Attack,
		Anim_Death,
		Anim_Falling,
		Anim_GetHit1,
		Anim_GetHit2,
		Anim_Idle,
		Anim_Jump,
		Anim_Jump_Flip,
		Anim_Landing,
		Anim_Revive,
		Anim_Running,
		Anim_Walking
	};

	// ========= エフェクト共通データ =========
	struct EffectData
	{
		Effect* effect = nullptr;

		DirectX::XMFLOAT3 offset = { 0.0f, 0.0f, 0.0f };
		float scale = 1.0f;

		bool autoPreview = false;
		float previewTimer = 0.0f;
		float previewInterval = 1.0f;

		float timer = 0.0f;
		float duration = 1.0f;

		float alpha = 1.0f;
		bool isPlaying = false;
	};

private:
	// ========= モデル =========
	Model* model = nullptr;
	bool drawModel = true;

	// ========= 移動 =========
	float moveSpeed = 5.0f;
	float turnSpeed = DirectX::XMConvertToRadians(720.0f);
	float jumpSpeed = 20.0f;

	int jumpCount = 0;
	int jumpLimit = 2;

	DirectX::XMFLOAT3 direction = { 0.0f, 0.0f, 1.0f };

	// ========= 出現状態 =========
	float appearTimer = 0.0f;
	float appearDelay = 3.0f;

	bool isAppeared = false;
	bool isAppearing = false;

	// ========= 弾管理 =========
	ProjectileManager projectileManager;

	// ========= デバッグ =========
	bool visibleDebugPrimitive = true;

	// ========= エフェクト =========
	EffectData appearanceFX;   // 出現演出
	EffectData hitFX;          // 被弾

};