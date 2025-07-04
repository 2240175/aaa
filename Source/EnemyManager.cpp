#include"EnemyManager.h"
#include"Collision.h"


//エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
	//破棄リストに追加
	removes.insert(enemy);
}


//更新処理
void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}

	//破棄処理
	//enemiesの範囲for文中でerase()すると不具合が発生してしまうため、
	//更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する
	for (Enemy* enemy : removes)
	{
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		//削除
		delete enemy;
	
	}

	//破棄リストをクリア
	removes.clear();

	//敵同士の衝突処理
	CollisionEnemyVsEnemies();
}

//描画処理
void EnemyManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{//すべての敵の
	for (Enemy* enemy : enemies)
	{
		
		enemy->Render(rc, renderer);
	}



}

//エネミー登録
void EnemyManager::Register(Enemy*enemy)
{
	enemies.emplace_back(enemy);
}



//エネミー全削除
void EnemyManager::Clear()
{

	for (Enemy* enemy : enemies)
	{
	
		delete enemy;

	}

	enemies.clear();

}
//デバッグプリミティブ描画
void EnemyManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
		for (Enemy* enemy : enemies)
	{

		enemy->RenderDebugPrimitive(rc,renderer);
	}

}

//エネミー同士の衝突処理
void EnemyManager::CollisionEnemyVsEnemies()
{	
	size_t enemyCount = enemies.size();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemyA = enemies.at(i);
		//敵のサイズを取得
		
		for (int j = i + 1; j  < enemyCount; ++j)
		{

			//i番目の敵
			Enemy* enemyB = enemies.at(j);

			DirectX::XMFLOAT3 outPosition;

			for (int j = i + 1; j < enemyCount; ++j)
			{
				//ｊ番目の敵を取得
				Enemy* enemyB = enemies.at(j);

				//衝突処理
				DirectX::XMFLOAT3 outPosition;
				//if (Collision::IntersectSphereVsSpgere(
				//	enemyA->GetPosition(),//j番目の敵の位置
				//	enemyA->GetRadius(),//ｊ番目の敵の半径
				//	enemyB->GetPosition(),//ｊ番目の敵の位置
				//	enemyB->GetRadius(),//ｊ番目の敵半径
				//	outPosition)) //押し出し位置（出力用）
				//{
				//		j番目の敵の位置を再設定（押し出し位置に設定する）

				//	enemyB->SetPosition(outPosition);

				//}
				if (Collision::IntersectCylinderVsCylinder(
					enemyA->GetPosition(),//i番目の敵の位置
					enemyA->GetRadius(),//i番目の敵の半径
					enemyA->GetHeight(),
					enemyB->GetPosition(),//j番目の敵の位置
					enemyB->GetRadius(),//j番目の敵の半径
					enemyB->GetHeight(),
					outPosition //押し出し位置（出力用）
					))
				{
					//j番目の敵の位置を再設定（押し出し位置に設定する）
					
					enemyB->SetPosition(outPosition);
				}

			}
		}

	}

}