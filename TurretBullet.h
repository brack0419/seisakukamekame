#pragma once

#include "Projectile.h"
#include "Graphics/Model.h"
class Enemy;

class TurretBullet : public Projectile
{
public:
    // コンストラクタ
    TurretBullet(ProjectileManager* manager, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& direction, Enemy* owner, Model* model);
    ~TurretBullet() override;

    void Update(float elapsedTime) override;

    void Render(const RenderContext& rc, ModelShader* shader) override;

private:
    float speed;
    float lifeTime;
    float timer;

    Model* model = nullptr;
    Enemy* owner = nullptr;
};