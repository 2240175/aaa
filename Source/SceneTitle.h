#pragma once
#include"System/Sprite.h"
#include"Scene.h"

//タイトルシーン
class SceneTitle : public Scene
{
public:
	SceneTitle(){}
	~SceneTitle() override {}

	//初期化
	void Initialize() override;

	//終了か
	void Finalize() override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render() override;

	//GUI描画
	void DrawGUI() override;


private:
	Sprite* sprite = nullptr;

};