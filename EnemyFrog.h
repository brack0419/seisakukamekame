#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

#define FROG_HP 10

// スライム
class EnemyFrog : public Enemy
{
public:
	// インスタンス取得
	static EnemyFrog& Instance();

	// 位置取得
	//const DirectX::XMFLOAT3& GetPosition() const { return position; }

	EnemyFrog();
	~EnemyFrog() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render(const RenderContext& rc, ModelShader* shader) override;
	// 追跡移動
	void TrackingMove(float elapsedTime);

	void Attack(float elapsedTime);

protected:
	// 死亡した時に呼ばれる
	void OnDead() override;
private:
	// アニメーション
	enum Animation
	{
		Anim_IdleNormal,
		Anim_IdleBattle,
		Anim_Attack01,
		Anim_Attack02,
		Anim_WalkFWD,
		Anim_WalkBWD,
		Anim_WalkLeft,
		Anim_WalkRight,
		Anim_RunFWD,
		Anim_SenseSomethingST,
		Anim_SenseSomethingRPT,
		Anim_Taunt,
		Anim_Victory,
		Anim_GetHit,
		Anim_Dizzy,
		Anim_Die,
	};

private:
	Model* model = nullptr;
	float	moveSpeed = 1.0f;
	float	turnSpeed = DirectX::XMConvertToRadians(720);

	float collsition = 0.8;

	float attck_time = 0;
};
