#include "ProjectileManager.h"

//コンストラクト
ProjectileManager::ProjectileManager()
{
}

//デストラクタ
ProjectileManager::~ProjectileManager()
{
	Clear();
}

//弾丸削除
void ProjectileManager::Remove(Projectile* projectile)
{
	//破棄リストに追加
	removes.insert(projectile);
}

//更新処理
void ProjectileManager::Update(float elapsedTime)
{
	//更新処理
	for (Projectile* projectile : projectiles)
	{

		projectile->Update(elapsedTime);
	}

	//破棄処理
	//projectilesの範囲for文中でerase()すると不具合が発生してしまうため、
	//　更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する
	for (Projectile* projectile : removes)
	{

		//std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);

		if (it != projectiles.end())
		{
			projectiles.erase(it);
		}
		//弾丸の破棄設定
		delete projectile;

	}
	//破棄リストをチェック
	removes.clear();
}

//描画処理
void ProjectileManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (Projectile* projectile : projectiles)
	{
		//描画処理を呼びだす
		projectile->Render(rc, renderer);
	}
}

//デバッグプリミティブ
void ProjectileManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//すべての敵の
	for (Projectile* projectile : projectiles)
	{

		projectile->RenderDebugPrimitive(rc, renderer);

	}

}

//弾丸登録
void ProjectileManager::Register(Projectile* projectile)
{
	projectiles.emplace_back(projectile);
}

//弾丸全削除
void ProjectileManager::Clear()
{
	//すべての敵
	for (Projectile* projectile : projectiles)
	{
		//破棄
		delete projectile;
	}

	projectiles.clear();

}

