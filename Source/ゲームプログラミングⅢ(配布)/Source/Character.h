#pragma once

#include<DirectXMath.h>
#include"System/ShapeRenderer.h"


//�L�����N�^�[
class Character
{
public:
	Character() {}
	virtual ~Character() {}



	//�ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	//�ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	//��]�擾
	const DirectX::XMFLOAT3& getAngle() const { return angle; }

	//��]�ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	//�X�P�[���擾
	const DirectX::XMFLOAT3& getScale() const { return scale; }

	//�X�P�[���ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	//���a�擾
	float GetRadius() const { return radius; }

	//�����擾
	float GetHeight() const { return height; }

	//�f�o�b�O�v���~�e�B�u�`��
	virtual void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* render);

	//�n�ʂɐڒn���Ă��邩
	bool IsGround() const { return isGround; }

	//�_���[�W��^����
	bool ApplyDamage(int damage,float invincibleTime);

	//�Ռ���^����
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

protected:
	//�ړ�����
	void Move(float elapsedTime, float vx, float vz, float speed);

	//���񏈗�
	void Turn(float elapsedTime, float vx, float vz, float speed);

	//�W�����v����
	void Jump(float speed);

	//���͏����X�V
	void UpdateVelocity(float elapsedTime);

	//���n�����Ƃ��ɌĂ΂��
	virtual void OnLanding() {}

	//�_���[�W���󂯂����̌Ă΂��
	virtual void OnDamage() {}

	//���S�����Ƃ��̌Ă΂��
	virtual void OnDead() {}

	//���G���ԑ���
	void UpdateInvincibleTimer(float elapsedTime);

private: 
	//�������͍X�V����
	void UpdateVerticalVelocity(float elapsedTime);

	//�����ړ��X�V����
	void UpdateVerticalMove(float elapsedTime);

	//�������͍X�V����
	void UpdateHorizontalVelocity(float elapsedTime);

	//�����ړ��X�V����
	void UpdateHorizontalMove(float elapsedTime);

protected:
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 angle = { 0,0,0 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };

	float radius = 0.5f;
	
	float gravity = -30.0f;
	DirectX::XMFLOAT3 velocity = { 0,0,0 };

	bool isGround = false;
	float height = 2.0f;

	int health = 5;
	float invincibleTimer = 1.0f;	

	float friction = 15.0f;


	float acceleration = 50.0f;
	float maxMoveSpeed = 5.0f;
	float moveVecX = 0.0f;
	float moveVecZ = 0.0f;

	float airControl = 0.3f;//�󒆐���
	
	
	//�s��X�V����
	void UpdateTransform();



};
