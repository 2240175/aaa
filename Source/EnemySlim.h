#pragma once

#include"System/Model.h"
#include"Enemy.h"

//�X���C��
class EnemySlime :public Enemy
{
public:
	EnemySlime();
	~EnemySlime() override;

	//�X�V����
	void Update(float elapsedTime) override;

	//�`�揈��
	void Render(const RenderContext& rc, ModelRenderer* renderer)override;

	//�f�o�b�O�v���~�e�B�u�`��
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)override;

	//�꒣��ݒ�
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);
private:
	//�^�[�Q�b�g�ʒu�������_���ݒ�
	void SetRandomTragetPosition();

	//�ړI�n�ֈړ�
	void MoveToTarget(float elapsedTime, float moveSpeedRate, float turnSpeedRate);

	//�p�j�X�e�[�g�֑J��
	void SetWanderState();

	//�p�j�X�e�[�g�X�V����
	void UpdateWanderState(float elapsedTime);

	//�ҋ@�X�e�[�g�֑J��
	void SetIdleState();

	//�ҋ@�X�e�[�g�X�V����
	void UpdateIdleState(float elapsedTime);

private:
	//�X�e�[�g
	enum class State
	{
		Wander,
		Idle
		
	};

	

protected: 
	//���S�����Ƃ��ɌĂ΂��
	void OnDead() override;

private:
	Model* model = nullptr;
	State					state = State::Wander;
	DirectX::XMFLOAT3		targetPosition = { 0,0,0 };
	DirectX::XMFLOAT3		territoryOrigin = { 0,0,0 };
	float					territoryRange = 10.0f;
	float					moveSpeed = 2.0f;
	float					turnSpeed = DirectX::XMConvertToRadians(360);
	float					stateTimer = 0.0f;
};