#include "EnemySlim.h"

#include "MathUtils.h"
#include "player.h"
#include "ProjectileStraight.h"

// コンストラクタ
EnemySlime::EnemySlime()
{
	model = new Model("Data/Model/Slime/Slime.mdl");

	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

	//幅・高さ設定
	radius = 0.5f;
	height = 1.0f;

	//徘徊ステートへ遷移
	SetWanderState();
}

// デストラクタ
EnemySlime::~EnemySlime()
{
	delete model;
}

// 更新処理
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

	case State::Attack:
		UodateAttackState(elapsedTime);
		break;
	}
	//速力処理更新
	UpdateVelocity(elapsedTime);

	//弾丸更新処理
	projectileManager.Update(elapsedTime);

	//無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列更新
	model->UpdateTransform();
}

// 描画処理
void EnemySlime::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);

	//弾丸描画処理
	projectileManager.Render(rc, renderer);
}

//死亡したときに呼ばれる
void EnemySlime::OnDead()
{
	//自身を破棄
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

	//索敵範囲をデバッグ円柱描画
	renderer->RenderCylinder(rc, position, searchRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
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
//プレイヤ-索敵
bool EnemySlime::SearchPlayer()
{
	//プレイヤーとの高低差を考慮して3Dでの距離判定する
	const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
	float vx = playerPosition.x - position.x;
	float vy = playerPosition.y - position.y;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist < searchRange)
	{
		float distXZ = sqrtf(vx * vx + vz * vz);
		//単位ベクトル化
		vx /= distXZ;
		vz /= distXZ;
		//前方ベクトル
		float  frontX = sinf(angle.y);
		float  frontZ = cosf(angle.y);
		//二つのベクトルの内積値で前後判定
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f)
		{
			return true;
		}
	}

	return false;
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
		//SetRandomTragetPosition();
		// 
		//待機ステートへ遷移
		SetIdleState();
	}
	//目的地点へ移動
	MoveToTarget(elapsedTime, 1.0f, 1.0f);
}
//待機ステートへ遷移
void EnemySlime::SetIdleState()
{
	state = State::Idle;

	//タイマーをランダム設定
	stateTimer = MathUtils::RandomRange(3.0f, 5.0f);
}

//待機ステート更新処理
void EnemySlime::UpdateIdleState(float elapsedTime)
{
	//タイマー処理
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		//徘徊ステートへ遷移
		SetWanderState();
	}
}

//攻撃ステートへ
void EnemySlime::SetAttackState()
{
	state = State::Attack;

	stateTimer = 0.0f;
}

void EnemySlime::UodateAttackState(float elapsedTime)
{
	//目的地店をプレイヤー位置に
	targetPosition = Player::Instance().GetPosition();

	//目標地点へ移動
	MoveToTarget(elapsedTime, 0.0f, 1.0f);

	//タイマー処理
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		//前方
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);
		dir.y = 0.0f;
		dir.z = cosf(angle.y);
		//発射位置
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y + height * 0.5f;
		pos.z = position.z;
		//発射
		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);

		stateTimer = 2.0f;
	}

	//プレイヤーを見失ったら
	if (!SearchPlayer())
	{
		SetIdleState();
	}
}






