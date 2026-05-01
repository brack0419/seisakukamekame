#include "EnemyFrog.h"
#include "Player.h"

static EnemyFrog* instance = nullptr;

// インスタンス取得
EnemyFrog& EnemyFrog::Instance()
{
	return *instance;
}

// コンストラクタ
EnemyFrog::EnemyFrog()
{
	// インスタンスポインタ設定
	instance = this;

	model = new Model("Data/Model/Frog/Frog1.mdl");

	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 1.0f;

	// 幅、高さ設定
	radius = 1.0f;
	height = 1.0f;
	model->PlayAnimation(Animation::Anim_IdleBattle, true);

	health = FROG_HP;
}

// デストラクタ
EnemyFrog::~EnemyFrog()
{
	delete model;
}

void EnemyFrog::Attack(float elapsedTime)
{
	Player* player = &player->Instance();
	DirectX::XMFLOAT3 target = player->GetPosition();
	DirectX::XMFLOAT3 frog = GetPosition();

	DirectX::XMVECTOR FrogPos = DirectX::XMVectorSet(frog.x, frog.y, frog.z, 0.0f);
	DirectX::XMVECTOR PlayerPos = DirectX::XMVectorSet(target.x, target.y, target.z, 0.0f);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PlayerPos, FrogPos);

	DirectX::XMVECTOR L = DirectX::XMVector3Length(Vec);
	float length;
	DirectX::XMStoreFloat(&length, L);

	if (length <= ((radius +0.5f)+player->GetRadius()))
	{
		attck_time += elapsedTime;
		if (attck_time >= 0.5f)
		{
			player->ApplyDamage(5, 0.0);
			attck_time = 0;
		}
		
	}
}

void EnemyFrog::TrackingMove(float elapsedTime)
{
	// 進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	// 移動処理
	Move(moveVec.x, moveVec.z, moveSpeed);

	// 旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

// 更新処理
void EnemyFrog::Update(float elapsedTime)
{
	TrackingMove(elapsedTime);

	Attack(elapsedTime);

	// 速力処理更新
	UpdateVelocity(elapsedTime);

	if (model->IsPlayAnimation() != Animation::Anim_WalkFWD)
	{
		model->PlayAnimation(Animation::Anim_WalkFWD, true);
	}

	// 無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	// オブジェクト行列を更新
	UpdateTransform();

	model->UpdateAnimation(elapsedTime);
	// モデル行列更新
	model->UpdateTransform(transform);
}

// 描画処理
void EnemyFrog::Render(const RenderContext& rc, ModelShader* shader)
{
	shader->Draw(rc, model);
}

// 死亡した時に呼ばれる
void EnemyFrog::OnDead()
{
	// 自身を破棄
	Destroy();
}