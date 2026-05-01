#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Collision.h"
#include "Effect.h"

// ステージ
class Stage
{
public:
	Stage();
	Stage(const char* filename);
	virtual ~Stage();

	// 更新処理
	virtual void Update(float elapsedTime);

	// 描画処理
	void Render(const RenderContext& rc, ModelShader* shader);

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

	// インスタンス取得
	static Stage& Instance();

	// デバッグ用GUI描画
	void DrawDebugGUI();

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	//セッター
	void SetPosition(DirectX::XMFLOAT3 p) { position = p; }
	void SetScale(DirectX::XMFLOAT3 s) { scale = s; }
	void SetAngle(DirectX::XMFLOAT3 a) { angle = a; }

	void UpdateTransform();

private:
	DirectX::XMFLOAT3 model1Scale = { 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 model2Scale = { 1.0f, 1.0f, 1.0f };
protected:
	std::shared_ptr<Model> model;
	std::shared_ptr<Model> model1;
	std::shared_ptr<Model> model2;
	DirectX::XMFLOAT4X4 transform;
	DirectX::XMFLOAT3 position{ 0,0,0 };
	DirectX::XMFLOAT3 scale{ 1,1,1 };
	DirectX::XMFLOAT3 angle{};
};

class Earth :public Stage
{
public:
	Earth();
	~Earth();

	// 更新処理
	void Update(float elapsedTime) override;
};
