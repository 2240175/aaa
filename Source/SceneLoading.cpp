#include"System/Graphics.h"
#include"System/Input.h"
#include"SceneLoading.h"
#include"SceneManager.h"

//初期化
void SceneLoading::Initialize()
{
	//	スプライト初期化
	sprite = new Sprite("Data/Sprite/LoadingIcon.png");

	//スレッド開始
	thread = new std::thread(LoadingThread, this);
}

//終了化
void SceneLoading::Finalize()
{
	//スレッド終了か
	if (thread != nullptr)
	{
		thread->join();
		delete thread;
		thread = nullptr;
	}

	//スプライト終了か
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
}

//更新処理
void SceneLoading::Update(float elapsedTime)
{
	constexpr float speed = 180;
	angle += speed * elapsedTime;

	//次のシーンの準備が完了したらシーンを切り替える
	if (nextScene->IsReady())
	{
		SceneManager::Instance().ChangeScene(nextScene);
			nextScene = nullptr;
	}
}

//描画処理
void SceneLoading::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();

	//描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();
	
	//2Dスプライト
	{
	
		//画面右下にローディングアイコン
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float spriteWidth = 256;
		float spriteHeight = 256;
		float positionX = screenWidth - spriteWidth;
		float positionY = screenHeight - spriteHeight;

		sprite->Render(rc, positionX, positionY, 0, spriteWidth, spriteHeight, angle, 1, 1, 1, 1);

	}
}

//GUI描画
void SceneLoading::DrawGUI()
{

}

//ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM関連
	CoInitialize(nullptr);

	//COM関連の初期化でスレッドごとに呼ぶ必要がある
	scene->nextScene->Initialize();

	//スレッドが終わる前にCOM関連の終了か
	CoUninitialize();

	//次のシーンの準備完了設定
	scene->nextScene->SetRedy();
}