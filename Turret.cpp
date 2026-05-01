#include "Turret.h"
#include "EnemyManager.h"
#include <windows.h>
#include <cmath>
#include "TurretBullet.h"
#include "imgui.h"
// コンストラクタ
Turret::Turret()
{

    model = new Model("Data/Model/Turret/Turret.mdl");

    bulletModel = new Model("Data/Model/Turret/Turret.mdl");

    scale.x = scale.y = scale.z = 3.0f;


}

// デストラクタ（消滅時の処理）
Turret::~Turret()
{
    delete model;

    if (bulletModel) {
        delete bulletModel;
    }
}


// 更新処理
void Turret::Update(float elapsedTime)
{
    // 速力更新と無敵時間の更新
    UpdateVelocity(elapsedTime);
    UpdateInvincibleTimer(elapsedTime);

    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    Enemy* targetEnemy = nullptr;
    float minDistance = 99999.0f;

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        if (enemy == this) continue;

        Turret* otherTurret = dynamic_cast<Turret*>(enemy);
        if (otherTurret != nullptr)
        {

            continue;
        }

        DirectX::XMFLOAT3 myPos = GetPosition();
        DirectX::XMFLOAT3 enemyPos = enemy->GetPosition();

        float dx = enemyPos.x - myPos.x;
        float dz = enemyPos.z - myPos.z;
        float distance = sqrtf(dx * dx + dz * dz);

        if (distance < minDistance)
        {
            minDistance = distance;
            targetEnemy = enemy;
        }
    }

    // --- モデルボーン回転処理 ---
    if (model)
    {
        model->UpdateAnimation(elapsedTime);


        Model::Node* neckNode = model->FindNode("sentry_base_03");
        if (neckNode != nullptr)
        {

            static bool isInitialized = false;
            static DirectX::XMFLOAT4 initialRotate;
            if (!isInitialized)
            {
                initialRotate = neckNode->rotate;
                isInitialized = true;
            }

            if (targetEnemy != nullptr)
            {
                DirectX::XMFLOAT3 myPos = GetPosition();
                DirectX::XMFLOAT3 targetPos = targetEnemy->GetPosition();

                float dirX = targetPos.x - myPos.x;
                float dirZ = targetPos.z - myPos.z;
                float targetAngleY = atan2f(dirX, dirZ);
                float localAngleY = targetAngleY - angle.y;


                float dirY = (targetPos.y + 1.0f) - myPos.y;
                float distXZ = sqrtf(dirX * dirX + dirZ * dirZ);
                float targetAngleX = atan2f(dirY, distXZ);

                // ③ 回転の合成（修正版）
                  // ---------------------------------------------------------
                DirectX::XMVECTOR qInit = DirectX::XMLoadFloat4(&initialRotate);
                DirectX::XMVECTOR qYaw = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), localAngleY);
                DirectX::XMVECTOR qPitch = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), targetAngleX);
                DirectX::XMVECTOR qRot = DirectX::XMQuaternionMultiply(qPitch, qYaw);
                DirectX::XMVECTOR qFinal = DirectX::XMQuaternionMultiply(qInit, qRot);
                DirectX::XMStoreFloat4(&neckNode->rotate, qFinal);

                fireTimer += elapsedTime;
                if (fireTimer >= fireInterval)
                {
                    fireTimer = 0.0f; // タイマーをリセット

                    // 弾の出現位置
                    DirectX::XMFLOAT3 myPos = GetPosition();
                    DirectX::XMFLOAT3 spawnPos = { myPos.x, myPos.y + 1.5f, myPos.z };


                    DirectX::XMFLOAT3 targetPos = targetEnemy->GetPosition();
                    DirectX::XMFLOAT3 dir;
                    dir.x = targetPos.x - spawnPos.x;
                    dir.y = (targetPos.y + 0.1f) - spawnPos.y; // 敵の中心付近を狙う
                    dir.z = targetPos.z - spawnPos.z;

                    // 方向ベクトル正規化
                    float len = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
                    if (len > 0.0f)
                    {
                        dir.x /= len;
                        dir.y /= len;
                        dir.z /= len;
                    }

                    spawnPos.y += 1.0f; // ← 高さを1上

                    TurretBullet* bullet = new TurretBullet(&projectileManager, spawnPos, dir, this, bulletModel);
                    // projectileManager.Register(bullet);
                }
            }
            else
            {
                // 敵がいないときは正面
                neckNode->rotate = initialRotate;
                fireTimer = 0.0f;
            }
        }
    }

    projectileManager.Update(elapsedTime);

    // 行列の更新
    UpdateTransform();

    if (model)
    {
        model->UpdateTransform(transform);
    }


}
// 描画処理
void Turret::Render(const RenderContext& rc, ModelShader* shader)
{
    if (model)
    {
        shader->Draw(rc, model);
    }

    projectileManager.Render(rc, shader);
}


void Turret::OnDead()
{

    Destroy();
}

void Turret::DrawDebugGUI()
{

    if (ImGui::TreeNode(this, "Turret"))
    {
        // 座標の変更
        DirectX::XMFLOAT3 pos = GetPosition();
        if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
        {
            SetPosition(pos);
        }

        ImGui::TreePop();
    }
}