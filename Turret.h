#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"
#include "ProjectileManager.h"


// タレット（固定砲台）クラス
class Turret : public Enemy
{
public:
    Turret();
    ~Turret() override;

    // 更新処理
    void Update(float elapsedTime) override;

    // 描画処理
    void Render(const RenderContext& rc, ModelShader* shader) override;

    void DrawDebugGUI() override;

protected:
    // 死亡した時に呼ばれる
    void OnDead() override;

private:
    Model* model = nullptr; // タレットの3Dモデル

    ProjectileManager projectileManager;
    float fireTimer = 0.0f;              // 発射からの経過時間を測るタイマー
    float fireInterval = 1.0f;           // 発射間隔
    Model* bulletModel = nullptr;
};
