#pragma once

#include"Stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"

// �Q�[���V�[��
class SceneGame:public Scene
{
public:
	SceneGame() {};
	~SceneGame() override  {  };

	// ������
	/*void Initialize();*/
	void Initialize() override;

	// �I����
	/*void Finalize();*/
	void Finalize() override;

	// �X�V����
	/*void Update(float elapsedTime);*/
	void Update(float elapsedTime)override;

	// �`�揈��
	/*void Render();*/
	void Render() override;

	// GUI�`��
	/*void DrawGUI();*/
	void DrawGUI() override;

private:
	Stage* stage = nullptr;
	Player* player = nullptr;
	CameraController * cameraController = nullptr;
};
