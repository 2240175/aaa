#include"EnemySlim.h"
#include"MathUtils.h"

//�R���X�g���N�^

EnemySlime::EnemySlime()
{
	model = new Model("Data/Model/Slime/Slime.mdl");

		//���f�����傫�̏o�X�P�[�����O
		scale.x = scale.y = scale.z = 0.01f;

		//���A�����ݒ�
		radius = 0.5f;
		height = 1.0f;

		//�p�j�X�e�[�g
		SetWanderState();
}

//�f�X�g���N�^
EnemySlime::~EnemySlime()
{
	delete model;
}

//�X�V����
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

	}

	//���͏���
	UpdateVelocity(elapsedTime);

	//���G���ԍX�V
	UpdateInvincibleTimer(elapsedTime);

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	//���f���s����X�V
	model->UpdateTransform();

}

//�`�揈��
void EnemySlime::Render(const RenderContext& rc, ModelRenderer* renderer)
{

	

	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

//���S�����Ƃ��̌Ă΂��
void EnemySlime::OnDead()
{
	//���M��j��
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
		SetRandomTragetPosition();
		
	}
	//�ړI�n�_�ֈړ�
	MoveToTarget(elapsedTime, 1.0f, 1.0f);
}