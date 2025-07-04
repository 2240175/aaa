#pragma once
#include<DirectXMath.h>

//kamera
class Camera
{
private:
	Camera() {}
	~Camera() {}

public:
	//�B��̃C���X�^���X�擾
	static Camera& Instance()
	{
		static Camera camera;
		return camera;
	}

	//���_�擾
	const DirectX::XMFLOAT3& GetEye() const { return eye; }

	//�����_�擾
	const DirectX::XMFLOAT3& GetFoucus() { return foucus; }

	//������擾
	const DirectX::XMFLOAT3& GetUp() const { return up; }
	
	//�O�����擾
	const DirectX::XMFLOAT3& GetFront() const { return front; }

	//�E����
	const DirectX::XMFLOAT3& GetRight() const { return right; }


	//�w������ɂނ�
	void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);

	//�p�[�X�y�N�e�B�u�ݒ�
	void SetPerspectiveFov(float forY, float aspect, float nearZ, float farZ);

	//�r���[�s��擾
	const DirectX::XMFLOAT4X4& GetView() const { return view; }

	//�v���W�F�N�g�����s��擾
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
