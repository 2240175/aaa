#pragma once
#include<DirectXMath.h>

//kamera
class Camera
{
private:
	Camera() {}
	~Camera() {}

public:
	//唯一のインスタンス取得
	static Camera& Instance()
	{
		static Camera camera;
		return camera;
	}

	//視点取得
	const DirectX::XMFLOAT3& GetEye() const { return eye; }

	//注視点取得
	const DirectX::XMFLOAT3& GetFoucus() { return foucus; }

	//上方向取得
	const DirectX::XMFLOAT3& GetUp() const { return up; }
	
	//前方向取得
	const DirectX::XMFLOAT3& GetFront() const { return front; }

	//右方向
	const DirectX::XMFLOAT3& GetRight() const { return right; }


	//指定方向にむく
	void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);

	//パースペクティブ設定
	void SetPerspectiveFov(float forY, float aspect, float nearZ, float farZ);

	//ビュー行列取得
	const DirectX::XMFLOAT4X4& GetView() const { return view; }

	//プロジェクトしょん行列取得
	const DirectX::XMFLOAT4X4& GetProjection() const { return projection; }

	private:
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;

		DirectX::XMFLOAT3 eye;
		DirectX::XMFLOAT3 foucus;

		DirectX::XMFLOAT3 up;
		DirectX::XMFLOAT3 front;
		DirectX::XMFLOAT3 right;


};
