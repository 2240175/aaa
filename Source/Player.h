#pragma once

#include"System/ModelRenderer.h"
#include"Character.h"
#include"ProjectileManager.h"


//�v���C���[
class Player : public Character
{
public:
	Player();
	~Player()override;

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(const RenderContext& rc, ModelRenderer* renderer);


	void DrawDebugGUI();

	//�C���X�^���X�擾
	static Player& Instance()
	{
		static Player instance;
		return instance;
	}
	

	//�W�����v���͏���
	void InputJump();

	//�f�o�b�O�v���~�e�B�u����
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)override;

	//�ړ����͏���
	void InputMove(float elapsedTime);

protected:
	//���n�����Ƃ��ɌĂ΂��
	void OnLanding() override;

private:
	

	Model* model = nullptr;
	//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
	DirectX::XMFLOAT3 GetMoveVec() const;

	//�v���C���[�ƃG�l�~�[�Ƃ̏Փ�
	void CollisionPlayerVsEnemies();

	//�e�ۓ��͏���
	void InputProjectile();

	//�e�ۂƓG�̏Փˏ���
	void CollisionProjectilesVsEnemies();


private:
	float moveSpeed = 5.0f;
	float turnSpeed = DirectX::XMConvertToRadians(720);
	float jumpSpeed = 12.0f; //�W�����v���x
	int jumpCount = 0;//���݂̃W�����v��
	int jumpLimit = 2;//�ő�W�����v��

	ProjectileManager projectileManager;
};

