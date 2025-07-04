#pragma once
#include"System/Sprite.h"
#include"Scene.h"
#include<thread>

//���[�f�B���O�V�[��
class SceneLoading : public Scene
{
public:
	/*SceneLoading() {}*/
	SceneLoading(Scene*nextScene):nextScene(nextScene){}
	~SceneLoading() override{}


	//������
	void Initialize() override;

	//�I����
	void Finalize() override;

	//�X�V����
	void Update(float elapsedTime)override;

	//�`�揈��
	void Render() override;

	//GUI�`��
	void DrawGUI() override;

private:
	Sprite* sprite = nullptr;
	float angle = 0.0f;

	static void LoadingThread(SceneLoading* scene);

private:
	Scene* nextScene = nullptr;
	std::thread* thread = nullptr;


};
