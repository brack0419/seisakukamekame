#pragma once

#include <vector>
#include <set>
#include "Enemy.h"

#define ENEMY_MAX 30

#define ENEMY_TYPE 2

// エネミーマネージャー
class EnemyManager
{
private:
	EnemyManager() {}
	~EnemyManager() {}

public:
	// 唯一のインスタンス取得
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}

	void Spawn(float elapsedTime);

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(const RenderContext& rc, ModelShader* shader);

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// エネミー登録
	void Register(Enemy* enemy);

	// エネミー全削除
	void Clear();

	// エネミー削除
	void Remove(Enemy* enemy);

	// エネミー数取得
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	// エネミー取得
	Enemy* GetEnemy(int index) { return enemies.at(index); }

	// デバッグ用GUI描画
	void DrawDebugGUI();

private:
	// エネミー同士の衝突処理
	void CollisionEnemyVsEnemies();

private:
	std::vector<Enemy*>		enemies;
	std::set<Enemy*>		removes;
	bool					visibleDebugPrimitive = true;

	int enemy_max = 0;
	float spwan_time = 0;

};
