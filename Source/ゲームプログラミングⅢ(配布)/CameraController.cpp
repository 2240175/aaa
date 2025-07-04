#include"System/Input.h"
#include"CameraController.h"
#include"Camera.h"

//�X�V����
void CameraController::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();

	//�J�����̉�]���x
	float speed = rollSpeed * elapsedTime;

	//�X�e�B�b�N�̓��͒l�ɕ�����X��Y����������b��
	angle.x += ay * speed;
	angle.y += ax * speed;

	//X���̃J������]�𐧌�
	if (angle.x < minAngleX)
	{
		angle.x = minAngleX;
	}
	if (angle.x > maxAngleX)
	{
		angle.x = maxAngleX;
	}

	//Y���̉�]�n���[�R�D�P�S�`�R�D�P�S�Ɏ��܂�悤
	if(angle.y < - DirectX::XM_PI)
	{ 
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}


	//�J������]�n����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//�����_(target)������x�N�g������(-front)�Ɉ�苗��(*range)���ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - front.x * range;
	eye.y = target.y - front.y * range;
	eye.z = target.z - front.z * range;

	//�J�����̎��_�ƒ����_��ݒ�
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}