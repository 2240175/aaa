#include"ProjectileHoming.h"

//コンストラクタ
ProjectileHoming::ProjectileHoming(ProjectileManager* manager):Projectile(manager)
{
	model = new Model("Data/Model/Sword/Sword.mdl");

	//モデルが小さいのでスケーリング
	scale.x = scale.y = scale.z = 3.0f;
}

//デストラクタ

ProjectileHoming::~ProjectileHoming()
{
	delete model;
}

//更新処理
void ProjectileHoming::Update(float elapsedTime)
{
	//寿命処理
	lifeTimer -= elapsedTime;
	if (lifeTimer <= 0.0f)
	{
		//自分を削除
		Destroy();
	
	}

	//移動
	{
		float moveSpeed = this->moveSpeed * elapsedTime;
		
		
		//位置＝ 位置　＋方向　＊　速さ
		position.x += direction.x * moveSpeed;//x
		position.y += direction.y * moveSpeed;//y
		position.z += direction.z * moveSpeed;//z

		////オブジェクト行列を更新
		//UpdateTransform();

		////モデル行列
		//model->UpdateTransform();

	
	}

	//旋回
	{
		float trunSpeed = this->turnSpeed * elapsedTime;
	
		//ターゲットまでのベクトルを計算
		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);//弾の位置（Position）をXMVECTORに変換
		DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);//ターゲットをXMVECTORに変換
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);//ターゲットー弾の位置

		//ゼロベクトルでないなら回転処理
		DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
		float lengthSq;
		DirectX::XMStoreFloat(&lengthSq, LengthSq);
		if (lengthSq > 0.00001f)
		{
		   //ターゲットまでのベクトルを単位ベクトル化(正規化)
			Vec = DirectX::XMVector3Normalize(Vec);

		   //向いてる方向ベクトルを算出
			DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);//前方向ベクトルをXMVETORに変換

		   //前方向ベクトルとターゲットまでのベクトルの内積（角度）を算出
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction,Vec);//directionと前方向ベクトル(Vec)の内遺

			float dot;
			DirectX::XMStoreFloat(&dot, Dot);
			

			//2つの単位ベクトルの角度が小さいほど
			//１．０に近づくという性質を利用して回転速度を調整する
			float rot = 1.0f - dot;
			if (rot > turnSpeed)
			{
			
				rot = turnSpeed;

			}


			//回転角度があるなら回転処理をする
			if (fabsf(rot) >0.0001f)
			{
			 //回転軸を算出
				DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction,Vec);
				Axis = DirectX::XMVector3Normalize(Axis);

				//回転軸と回転量から回転行列を算出
				DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis,rot);

				//現在の行列を回転させる
				DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
				Transform = DirectX::XMMatrixMultiply(Transform,Rotation);

				//回転後の前方方向を取り戻し、単位ベクトル化する
				Direction = DirectX::XMVector3Normalize(Transform.r[2]);
				DirectX::XMStoreFloat3(&direction, Direction);

			}
		
		}

		//オブジェクト行列を更新
		UpdateTransform();

		//モデル行列更新
		model->UpdateTransform();
	}
}

//描画処理
void ProjectileHoming::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}
//発射

//発射
void ProjectileHoming::Launch(const DirectX::XMFLOAT3& direction,
	const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& target)
{

	this->direction = direction;
	this->position = position;
	this->target = target;

	UpdateTransform();

}