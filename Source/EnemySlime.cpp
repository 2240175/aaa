#include"EnemySlim.h"
#include"MathUtils.h"

//コンストラクタ

EnemySlime::EnemySlime()
{
	model = new Model("Data/Model/Slime/Slime.mdl");

		//モデルが大きの出スケーリング
		scale.x = scale.y = scale.z = 0.01f;

		//幅、高さ設定
		radius = 0.5f;
		height = 1.0f;

		//徘徊ステート
		SetWanderState();
}

//デストラクタ
EnemySlime::~EnemySlime()
{
	delete model;
}

//更新処理
void EnemySlime::Update(float elapsedTime)
{
	//ステート毎の更新処理
	switch (state)
	{
	case State::Wander:
		UpdateWanderState(elapsedTime);
		break;

	case State::Idle:
		UpdateIdleState(elapsedTime);
		break;

	}

	//速力処理
	UpdateVelocity(elapsedTime);

	//無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列を更新
	model->UpdateTransform();

}

//描画処理
void EnemySlime::Render(const RenderContext& rc, ModelRenderer* renderer)
{

	

	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

//死亡したときの呼ばれる
void EnemySlime::OnDead()
{
	//自信を破棄
	Destroy();
}


//デバッグプリミティブ描画
void EnemySlime::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//規定クラスのデバッグプリミティブ描画
	Enemy::RenderDebugPrimitive(rc, renderer);

	//縄張り範囲をデバッグ円柱描画
	renderer->RenderCylinder(rc, territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0, 1, 0, 1));

	//ターゲット位置をデバッグ球描画
	renderer->RenderSphere(rc, targetPosition, 1.0f, DirectX::XMFLOAT4(1, 1, 0, 1));

	
}

//縄張り設定
void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}


//ターゲット位置をランダム設定
void EnemySlime::SetRandomTragetPosition()
{
	float theta = MathUtils::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = MathUtils::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

//目的地へ移動
void EnemySlime::MoveToTarget(float elapsedTime, float moveSpeedRate, float turnSpeedRate)
{
	//ターゲット方向への進行ベクトル算出
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	//移動処理
	Move(elapsedTime, vx, vz, moveSpeed * moveSpeedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * turnSpeedRate);
}

//徘徊ステートへ遷移
void EnemySlime::SetWanderState()
{
	state = State::Wander;

	//目的地点設定
	SetRandomTragetPosition();
}

//徘徊ステート更新処理
void EnemySlime::UpdateWanderState(float elapsedTime)
{
	//目的地点までXZ平面での距離判定
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq < radius * radius)
	{
		//次の目的地点設定
		SetRandomTragetPosition();
		
	}
	//目的地点へ移動
	MoveToTarget(elapsedTime, 1.0f, 1.0f);
}