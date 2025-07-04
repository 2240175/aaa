#pragma once
#include"System/ModelRenderer.h"
#include"Character.h"

class Enemy : public Character
{
public:
	Enemy() {}
	~Enemy() {}

	//XVˆ—
	virtual void Update(float elapsedTime) = 0;

	//•`‰æˆ—
	virtual void Render(const RenderContext& rc, ModelRenderer* renerer) = 0;

	//”jŠü
	void Destroy();

};