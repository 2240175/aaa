#pragma once

#include <vector>
#include "Projectile.h"
#include<set>

//�e�ۃ}�l�[�W���[
class ProjectileManager
{
public:
	ProjectileManager();
	~ProjectileManager();

	//�X�V����
	void Update(float elapedTime);

	//�`�揈��
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//�f�o�b�O�v���~�e�B�u�`��
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	//�e�ۓo�^
	void Register(Projectile* projectile);

	//�e�ۑS�폜
	void Clear();

	//�e�ې��擾
	int GetProjectileCount() const { return static_cast<int>(projectiles.size()); }

	//�e�ێ擾
	Projectile*GetProjectile(int index) { return projectiles.at(index); }

	//�e�ۍ폜
	void Remove(Projectile* projectile);

private:
	std::vector<Projectile*> projectiles;
	std::set<Projectile*> removes;



};
