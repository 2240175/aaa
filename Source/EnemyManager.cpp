#include"EnemyManager.h"
#include"Collision.h"


//�G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy)
{
	//�j�����X�g�ɒǉ�
	removes.insert(enemy);
}


//�X�V����
void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}

	//�j������
	//enemies�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	//�X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����
	for (Enemy* enemy : removes)
	{
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		//�폜
		delete enemy;
	
	}

	//�j�����X�g���N���A
	removes.clear();

	//�G���m�̏Փˏ���
	CollisionEnemyVsEnemies();
}

//�`�揈��
void EnemyManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{//���ׂĂ̓G��
	for (Enemy* enemy : enemies)
	{
		
		enemy->Render(rc, renderer);
	}



}

//�G�l�~�[�o�^
void EnemyManager::Register(Enemy*enemy)
{
	enemies.emplace_back(enemy);
}



//�G�l�~�[�S�폜
void EnemyManager::Clear()
{

	for (Enemy* enemy : enemies)
	{
	
		delete enemy;

	}

	enemies.clear();

}
//�f�o�b�O�v���~�e�B�u�`��
void EnemyManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
		for (Enemy* enemy : enemies)
	{

		enemy->RenderDebugPrimitive(rc,renderer);
	}

}

//�G�l�~�[���m�̏Փˏ���
void EnemyManager::CollisionEnemyVsEnemies()
{	
	size_t enemyCount = enemies.size();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemyA = enemies.at(i);
		//�G�̃T�C�Y���擾
		
		for (int j = i + 1; j  < enemyCount; ++j)
		{

			//i�Ԗڂ̓G
			Enemy* enemyB = enemies.at(j);

			DirectX::XMFLOAT3 outPosition;

			for (int j = i + 1; j < enemyCount; ++j)
			{
				//���Ԗڂ̓G���擾
				Enemy* enemyB = enemies.at(j);

				//�Փˏ���
				DirectX::XMFLOAT3 outPosition;
				//if (Collision::IntersectSphereVsSpgere(
				//	enemyA->GetPosition(),//j�Ԗڂ̓G�̈ʒu
				//	enemyA->GetRadius(),//���Ԗڂ̓G�̔��a
				//	enemyB->GetPosition(),//���Ԗڂ̓G�̈ʒu
				//	enemyB->GetRadius(),//���Ԗڂ̓G���a
				//	outPosition)) //�����o���ʒu�i�o�͗p�j
				//{
				//		j�Ԗڂ̓G�̈ʒu���Đݒ�i�����o���ʒu�ɐݒ肷��j

				//	enemyB->SetPosition(outPosition);

				//}
				if (Collision::IntersectCylinderVsCylinder(
					enemyA->GetPosition(),//i�Ԗڂ̓G�̈ʒu
					enemyA->GetRadius(),//i�Ԗڂ̓G�̔��a
					enemyA->GetHeight(),
					enemyB->GetPosition(),//j�Ԗڂ̓G�̈ʒu
					enemyB->GetRadius(),//j�Ԗڂ̓G�̔��a
					enemyB->GetHeight(),
					outPosition //�����o���ʒu�i�o�͗p�j
					))
				{
					//j�Ԗڂ̓G�̈ʒu���Đݒ�i�����o���ʒu�ɐݒ肷��j
					
					enemyB->SetPosition(outPosition);
				}

			}
		}

	}

}