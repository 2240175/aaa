#pragma once

#include"System/ModelRenderer.h"
#include"System/ShapeRenderer.h"

//前方宣言
class ProjectileManager;

//弾丸
class Projectile
{
public:
	Projectile(ProjectileManager* manager);
	virtual ~Projectile() {}

	//更新処理
	virtual void Update(float elapsedTime) = 0;

	//描画処理
	virtual void Render(const RenderContext& rc, ModelRenderer* renderer) = 0;

	//デバックプリミティブ描画
	virtual void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	//位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	//方向取得
	const DirectX::XMFLOAT3& GetDirection() const { return direction; }

	//スケール取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	//半径取得
	float GetRadius() const { return radius; }

	//破棄
	void Destroy();

protected:
	//行列更新処理
	void UpdateTransform();

protected:
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 direction = { 0,0,0 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

	ProjectileManager* manager = nullptr;

	float radius = 0.5f;
};

