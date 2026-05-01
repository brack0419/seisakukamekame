#include "Stage.h"
#include "Graphics/Graphics.h"

// コンストラクタ
Stage::Stage()
{
	model = std::make_shared<Model>("Data/Model/Stage/yuka.mdl");
	//model1 = std::make_shared<Model>("Data/Model/Stage/stage1.mdl");
}

Stage::Stage(const char* filename)
{
	model = std::make_shared<Model>(filename);
	//model1 = std::make_shared<Model>(filename);
}

Stage::~Stage()
{
}

// 更新
void Stage::Update(float elapsedTime)
{
	using namespace DirectX;

	// model用
	UpdateTransform();
	model->UpdateTransform(transform);

	// model1用
	XMMATRIX S = XMMatrixScaling(
		model1Scale.x,
		model1Scale.y,
		model1Scale.z
	);

	XMMATRIX R = XMMatrixRotationRollPitchYaw(
		angle.x, angle.y, angle.z
	);

	XMMATRIX T = XMMatrixTranslation(
		position.x, position.y, position.z
	);

	XMMATRIX W = S * R * T;

	DirectX::XMFLOAT4X4 model1Transform;
	XMStoreFloat4x4(&model1Transform, W);

	//model1->UpdateTransform(model1Transform);
}

// レイキャスト（yuka 用）
bool Stage::RayCast(const DirectX::XMFLOAT3& start,
	const DirectX::XMFLOAT3& end,
	HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model.get(), hit);
}

// 描画
void Stage::Render(const RenderContext& rc, ModelShader* shader)
{
	shader->Draw(rc, model.get());
	//shader->Draw(rc, model1.get());
}

// Transform 更新
void Stage::UpdateTransform()
{
	using namespace DirectX;

	XMMATRIX S = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX R = XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	XMMATRIX T = XMMatrixTranslation(position.x, position.y, position.z);

	XMMATRIX W = S * R * T;
	XMStoreFloat4x4(&transform, W);
}

void Stage::DrawDebugGUI()
{
#ifdef _DEBUG

#endif
}

///////////////////////////////////////////////////////////////////

// コンストラクタ
Earth::Earth()
{
}

Earth::~Earth()
{
}

// 更新処理
void Earth::Update(float elapsedTime)
{
	Stage::UpdateTransform();
	model->UpdateTransform(transform);
}