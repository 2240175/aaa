#pragma once
#include"System/ModelRenderer.h"
#include"Character.h"

class Enemy : public Character
{
public:
	Enemy() {}
	~Enemy() {}

	//�X�V����
	virtual void Update(float elapsedTime) = 0;

	//�`�揈��
	virtual void Render(const RenderContext& rc, ModelRenderer* renerer) = 0;

	//�j��
	void Destroy();

};