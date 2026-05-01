#include "TurretBullet.h"
#include "EnemyManager.h"
#include <cmath>
#include "Turret.h"
// コンストラクタ
TurretBullet::TurretBullet(ProjectileManager* manager, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& direction, Enemy* owner, Model* model)
    : Projectile(manager)
{
    this->owner = owner;
    this->model = model;
    this->position = position;
    this->direction = direction;
    radius = 0.2f;// 当たり判定
    lifeTime = 115.0f;

    speed = 13.0f;// 弾速
    timer = 0.0f;  // タイマーを初期化
    scale.x = scale.y = scale.z = 0.5f;
}

// デストラクタ
TurretBullet::~TurretBullet()
{

}

// 毎フレーム呼ばれる更新処理
void TurretBullet::Update(float elapsedTime)
{
    //  寿命
    timer += elapsedTime;
    if (timer >= lifeTime)
    {
        Destroy();
        return;
    }

    //  移動処理
    position.x += direction.x * speed * elapsedTime;
    position.y += direction.y * speed * elapsedTime;
    position.z += direction.z * speed * elapsedTime;

    // 行列の更新（
    UpdateTransform();

    //  敵との当たり判定
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        if (enemy == this->owner)
        {
            continue;
        }

        Turret* hitTurret = dynamic_cast<Turret*>(enemy);
        if (hitTurret != nullptr)
        {
            continue;
        }


        // 敵の位置を取得
        DirectX::XMFLOAT3 enemyPos = enemy->GetPosition();

        // 敵の当たり判定の半径を取得
        float enemyRadius = enemy->GetRadius();

        float dx = position.x - enemyPos.x;
        float dy = position.y - enemyPos.y;
        float dz = position.z - enemyPos.z;
        float distance = sqrtf(dx * dx + dy * dy + dz * dz);


        if (distance <= (this->radius + enemyRadius))
        {

            // OutputDebugStringA("HIT Enemy!\n");

            Destroy();
            break;
        }
    }
}

// 描画処理
void TurretBullet::Render(const RenderContext& rc, ModelShader* shader)
{

    if (model)
    {
        model->UpdateTransform(transform);

        shader->Draw(rc, model);
    }

    DrawDebugPrimitive();
}