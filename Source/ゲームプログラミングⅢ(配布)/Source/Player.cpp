#include "Player.h"
#include "System/Input.h"
#include "Camera.h"
#include<imgui.h>
#include"EnemyManager.h"
#include"Collision.h"
#include"ProjectileStraight.h"
#include"ProjectileManager.h"
#include"ProjectileHoming.h"

//コンストラクタ
Player::Player()
{
	model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

}

//デストラクタ
Player::~Player()
{
	delete model;
}

//更新処理
void Player::Update(float elapsedTime)
{
	////進行ベクトル取得
	//DirectX::XMFLOAT3 moveVec = GetMoveVec();

	////移動処理
	//float moveSpeed = this->moveSpeed * elapsedTime;
	//position.x += moveVec.x * moveSpeed;
	//position.z += moveVec.z * moveSpeed;

	//移動入力処理
	InputMove(elapsedTime);

	//ジャンプ入力処理
	InputJump();

    //弾丸更新処理
	InputProjectile();
	
	//速力処理更新
	UpdateVelocity(elapsedTime);

	// 弾丸更新処理
	projectileManager.Update(elapsedTime);


	//プレイヤーと敵との衝突
	CollisionPlayerVsEnemies();

	//弾丸と敵の衝突処理
	CollisionProjectilesVsEnemies();

	//オブジェクト行列の更新
	UpdateTransform();

	//モデル行列更新
	model->UpdateTransform();
	
	
	

}

void Player::OnLanding()
{
	jumpCount = 0;
}



//移動入力処理
void Player::InputMove(float elapsedTime)
{
	//進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	//移動処理
	Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);
	//旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

}

//描画処理
void Player::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);

	//弾丸描画処理
	projectileManager.Render(rc,renderer);
}

//デバッグプリミティブ描画
void Player::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//基底クラスの呼び出し
	Character::RenderDebugPrimitive(rc, renderer);


	//弾丸デバッグプリミティブ描画
	projectileManager.RenderDebugPrimitive(rc, renderer);
}

void Player::DrawDebugGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		//トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//位置
			ImGui::InputFloat3("Position", &position.x);
			//回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			//スケール
			ImGui::InputFloat3("Scale", &scale.x);
		}
	}


	ImGui::End();
}

//スティック入力値から移動べくとるを取得
DirectX::XMFLOAT3 Player::GetMoveVec()const
{
	//入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	//カメラ方向とスティック入力値によって進行方向を計算する
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//移動ベクトルはXZ平面に水平なベクトルになるようにする

	//カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	//カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		//単位ベクトル
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;

	}

	//ステック水平入力値をカメラ右方向にはんえいし、
	//スティック垂直入力値をカメラ前方向に反映し、
	//進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
	vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);

	//Y軸には移動しない
	vec.y = 0.0f;
	return vec;
}

//プレイヤーとエネミーの衝突処理
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//すべての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//衝突処理
		DirectX::XMFLOAT3 outPosition;

		//if (Collision::IntersectSphereVsSpgere(
		//position,
		//radius,
		//enemy->GetPosition(),
		//enemy->GetRadius(),
		//outPosition))
		//{
		//	//押し出し後の位置設定
		//	enemy->SetPosition(outPosition);
		//}

		if (Collision::IntersectCylinderVsCylinder(
			position,
			radius,
			height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition))
		{
		
			////押し出し後の位置設定
			//enemy->SetPosition(outPosition);

			//敵の真上付近に当たったかを判定
			//プレイヤーの位置をXMVECTORに変換
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			//敵の位置をXMVECTORに変換
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			//敵からプレイヤーへのベクトル計算
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			//そのベクトルを正規化（単位ベクトル化）
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			//正規化にしたベクトルをXMFLOAT3に変換
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, N);
			//上から踏んづけたばあいは小ジャンプする
			if (normal.y > 0.8f)
			{
				//小ジャンプする
				Jump(jumpSpeed * 0.5f);
			}
			else
			{
				//押し出し後の位置設定（上から踏んづけなかった場合の処理）
				enemy->SetPosition(outPosition);
			}
		
		}


	}
}

//弾丸と敵の衝突
void Player::CollisionProjectilesVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//すべての弾丸とすべての敵を総当たりで衝突処理
	int projectileCount = projectileManager.GetProjectileCount();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < projectileCount; ++i)
	{
	
		Projectile* projectile = projectileManager.GetProjectile(i);

		for (int j = 0; j < enemyCount; ++j)
		{
		
			Enemy* enemy = enemyManager.GetEnemy(j);

			//衝突処理
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsCylinder(
				projectile->GetPosition(),
				projectile->GetRadius(),
				enemy->GetPosition(),
				enemy->GetRadius(),
				enemy->GetHeight(),
				outPosition))
			{
			
				//ダメージを与える
				if (enemy->ApplyDamage(1, 0.5f))
				{
					//吹き飛ばす
					{
						DirectX::XMFLOAT3 impulse;//衝撃ベクトル
						const float power = 10.0f;
						const DirectX::XMFLOAT3& e = enemy->GetPosition();
						const DirectX::XMFLOAT3& p = projectile->GetPosition();
						float vx = e.x - p.x;
						float vz = e.z - p.z;
						float lenghtXZ = sqrtf(vx * vx + vz * vz);
						vx /= lenghtXZ;
						vz /= lenghtXZ;

						impulse.x = vx * power;
						impulse.y =  power * 0.5f;
						impulse.z = vz * power;

						enemy->AddImpulse(impulse);
					}

					//弾丸破棄
					projectile->Destroy();
				}
			}

		}
	
	}

}


//ジャンプ入力処理
void Player::InputJump()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		//ジャンプ回数制限（現在のジャンプ回数がジャンプの最大数より小さければ）
		if (jumpCount < jumpLimit)
		{

			//現在のジャンプ数を増加させ
			jumpCount++;

			//ジャンプ
			Jump(jumpSpeed);

		}
	}
}

//弾丸入力処理
void Player::InputProjectile()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//直進弾丸発射
	if (gamePad.GetButtonDown() & GamePad::BTN_X)
	{
	//前方向
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);//前方向(sinとangleで計算)
		dir.y = 0.0f;
		dir.z = cosf(angle.y);//前方向(cosとangleで計算)
		

	//発射位置（プレイヤーの腰あたりyがheightの半分）
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y +height *0.5f;
		pos.z = position.z;

		//発射
		ProjectileStraight * projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
	}

	//追尾弾丸発射
	if (gamePad.GetButtonDown() & GamePad::BTN_Y)
	{
	//前方向
	
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);//前方向(sinとangleで計算)
		dir.y = 0.0f;
		dir.z = cosf(angle.y);//前方向(cosとangleで計算)

	//発射位置
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
			pos.y = position.y + height * 0.5f;
			pos.z = position.z;

	//ターゲット（デフォルトではないプレいやーの前方）
			DirectX::XMFLOAT3 target;
			target.x = pos.x + dir.x * 1000.0f;
			target.y = pos.y + dir.y * 1000.0f;
			target.z = pos.z + dir.z * 1000.0f;

	float dist = FLT_MAX;
		EnemyManager& enemyManager = EnemyManager::Instance();
		int enemyCount = enemyManager.GetEnemyCount();
		for (int i = 0; i < enemyCount; i++)
		{
			//敵との距離判定
			Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);//弾の位置
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());//敵の位置
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E,P);//敵の位置　ー　弾の位置
			DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);//敵への方向ベクトルの長さ
			float d;//今回の距離（弾から敵までの距離）
			DirectX::XMStoreFloat(&d, D);
			//今回の敵のほうが弾に近いかどうかチェック
			if (d < dist)
			{
			//近かったらtargetに設定
				dist = d;
				target = enemy->GetPosition();
				target.y += enemy->GetHeight() * 0.5f;
			
			}
		
		}

		//発射
		ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
		projectile->Launch(dir, pos, target);
	}
}
