#pragma once
#include"System/Sprite.h"
#include"Scene.h"
#include<thread>

//ローディングシーン
class SceneLoading : public Scene
{
public:
	/*SceneLoading() {}*/
	SceneLoading(Scene*nextScene):nextScene(nextScene){}
	~SceneLoading() override{}


	//初期化
	void Initialize() override;

	//終了か
	void Finalize() override;

	//更新処理
	void Update(float elapsedTime)override;

	//描画処理
	void Render() override;

	//GUI描画
	void DrawGUI() override;

private:
	Sprite* sprite = nullptr;
	float angle = 0.0f;

	static void LoadingThread(SceneLoading* scene);

private:
	Scene* nextScene = nullptr;
	std::thread* thread = nullptr;


};
