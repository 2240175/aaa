#include"Stage.h"

//コンストラクタ
Stage::Stage()
{
	//ステージモデルを読み込む
	model = new Model("Data/Model/Stage/ExampleStage.mdl");

}

Stage::~Stage()
{
	//ステージモデルを破棄
	delete model;

}

//更新処理
void Stage::Update(float elapsedTime)
{
//今はやることはない
}

//描画処理
void Stage::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	DirectX::XMFLOAT4X4 transform;
	DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixIdentity());


	//レンダラにモデルを描画してもらう
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}