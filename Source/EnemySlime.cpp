#include "EnemySlim.h"

#include "MathUtils.h"
#include "player.h"
#include "ProjectileStraight.h"

// �R���X�g���N�^
EnemySlime::EnemySlime()
{
	model = new Model("Data/Model/Slime/Slime.mdl");

	// ���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

	//���E�����ݒ�
	radius = 0.5f;
	height = 1.0f;

	//�p�j�X�e�[�g�֑J��
	SetWanderState();
}

// �f�X�g���N�^
EnemySlime::~EnemySlime()
{
	delete model;
}

// �X�V����
void EnemySlime::Update(float elapsedTime)
{
	//�X�e�[�g���̍X�V����
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
	//���͏����X�V
	UpdateVelocity(elapsedTime);

	//�e�ۍX�V����
	projectileManager.Update(elapsedTime);

	//���G���ԍX�V
	UpdateInvincibleTimer(elapsedTime);

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
	model->UpdateTransform();
}

// �`�揈��
void EnemySlime::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);

	//�e�ە`�揈��
	projectileManager.Render(rc, renderer);
}

//���S�����Ƃ��ɌĂ΂��
void EnemySlime::OnDead()
{
	//���g��j��
	Destroy();
}

//�f�o�b�O�v���~�e�B�u�`��
void EnemySlime::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//�K��N���X�̃f�o�b�O�v���~�e�B�u�`��
	Enemy::RenderDebugPrimitive(rc, renderer);

	//�꒣��͈͂��f�o�b�O�~���`��
	renderer->RenderCylinder(rc, territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0, 1, 0, 1));

	//�^�[�Q�b�g�ʒu���f�o�b�O���`��
	renderer->RenderSphere(rc, targetPosition, 1.0f, DirectX::XMFLOAT4(1, 1, 0, 1));

	//���G�͈͂��f�o�b�O�~���`��
	renderer->RenderCylinder(rc, position, searchRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}
//�꒣��ݒ�
void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}

//�^�[�Q�b�g�ʒu�������_���ݒ�
void EnemySlime::SetRandomTragetPosition()
{
	float theta = MathUtils::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = MathUtils::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

//�ړI�n�ֈړ�
void EnemySlime::MoveToTarget(float elapsedTime, float moveSpeedRate, float turnSpeedRate)
{
	//�^�[�Q�b�g�����ւ̐i�s�x�N�g���Z�o
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	//�ړ�����
	Move(elapsedTime, vx, vz, moveSpeed * moveSpeedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * turnSpeedRate);
}
//�v���C��-���G
bool EnemySlime::SearchPlayer()
{
	//�v���C���[�Ƃ̍��፷���l������3D�ł̋������肷��
	const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
	float vx = playerPosition.x - position.x;
	float vy = playerPosition.y - position.y;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist < searchRange)
	{
		float distXZ = sqrtf(vx * vx + vz * vz);
		//�P�ʃx�N�g����
		vx /= distXZ;
		vz /= distXZ;
		//�O���x�N�g��
		float  frontX = sinf(angle.y);
		float  frontZ = cosf(angle.y);
		//��̃x�N�g���̓��ϒl�őO�㔻��
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f)
		{
			return true;
		}
	}

	return false;
}

//�p�j�X�e�[�g�֑J��
void EnemySlime::SetWanderState()
{
	state = State::Wander;

	//�ړI�n�_�ݒ�
	SetRandomTragetPosition();
}

//�p�j�X�e�[�g�X�V����
void EnemySlime::UpdateWanderState(float elapsedTime)
{
	//�ړI�n�_�܂�XZ���ʂł̋�������
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq < radius * radius)
	{
		//���̖ړI�n�_�ݒ�
		//SetRandomTragetPosition();
		// 
		//�ҋ@�X�e�[�g�֑J��
		SetIdleState();
	}
	//�ړI�n�_�ֈړ�
	MoveToTarget(elapsedTime, 1.0f, 1.0f);
}
//�ҋ@�X�e�[�g�֑J��
void EnemySlime::SetIdleState()
{
	state = State::Idle;

	//�^�C�}�[�������_���ݒ�
	stateTimer = MathUtils::RandomRange(3.0f, 5.0f);
}

//�ҋ@�X�e�[�g�X�V����
void EnemySlime::UpdateIdleState(float elapsedTime)
{
	//�^�C�}�[����
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		//�p�j�X�e�[�g�֑J��
		SetWanderState();
	}
}

//�U���X�e�[�g��
void EnemySlime::SetAttackState()
{
	state = State::Attack;

	stateTimer = 0.0f;
}

void EnemySlime::UodateAttackState(float elapsedTime)
{
	//�ړI�n�X���v���C���[�ʒu��
	targetPosition = Player::Instance().GetPosition();

	//�ڕW�n�_�ֈړ�
	MoveToTarget(elapsedTime, 0.0f, 1.0f);

	//�^�C�}�[����
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		//�O��
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);
		dir.y = 0.0f;
		dir.z = cosf(angle.y);
		//���ˈʒu
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y + height * 0.5f;
		pos.z = position.z;
		//����
		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);

		stateTimer = 2.0f;
	}

	//�v���C���[������������
	if (!SearchPlayer())
	{
		SetIdleState();
	}
}






