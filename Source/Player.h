#pragma once

#include"System/ModelRenderer.h"
#include"Character.h"
#include"ProjectileManager.h"


//プレイヤー
class Player : public Character
{
public:
	Player();
	~Player()override;

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);


	void DrawDebugGUI();

	//インスタンス取得
	static Player& Instance()
	{
		static Player instance;
		return instance;
	}
	

	//ジャンプ入力処理
	void InputJump();

	//デバッグプリミティブ処理
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)override;

	//移動入力処理
	void InputMove(float elapsedTime);

protected:
	//着地したときに呼ばれる
	void OnLanding() override;

private:
	

	Model* model = nullptr;
	//スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;

	//プレイヤーとエネミーとの衝突
	void CollisionPlayerVsEnemies();

	//弾丸入力処理
	void InputProjectile();

	//弾丸と敵の衝突処理
	void CollisionProjectilesVsEnemies();


private:
	float moveSpeed = 5.0f;
	float turnSpeed = DirectX::XMConvertToRadians(720);
	float jumpSpeed = 12.0f; //ジャンプ速度
	int jumpCount = 0;//現在のジャンプ数
	int jumpLimit = 2;//最大ジャンプ数

	ProjectileManager projectileManager;
};

