#pragma once

#include<vector>
#include<set>
#include"Enemy.h"


//エネミーマネージャー
class EnemyManager
{
private:
	EnemyManager() {}
	~EnemyManager() {}

public:
	//唯一のインスタンス取得
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);

	//更新処理
	void Render(const RenderContext& rc, ModelRenderer * renderer);

	//エネミー登録
	void Register(Enemy* enemy);

	//エネミー全削除
	void Clear();

	//デバッグプリミティブ描画
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	//エネミー数取得
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	//エネミー取得
	Enemy* GetEnemy(int index) { return enemies.at(index); }

	//エネミー削除
	void Remove(Enemy* enemy);	
	
	//エネミー同士の衝突
	void CollisionEnemyVsEnemies();



private:
	std::vector<Enemy*> enemies;
	std::set<Enemy*> removes;



};