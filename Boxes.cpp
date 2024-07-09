#include "Boxes.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "Collision.h"

Boxes::Boxes()
{
	model = new Model("Data/Model/Cube/Cube.mdl");

	//スケーリング
	scale.x = scale.y = scale.z = 0.01f;

	//幅、高さ
	radius = 0.5f;
	height = 1.0f;

	
}

Boxes::~Boxes()
{
	delete model;
}

void Boxes::Update(float elapsedTime)
{
	

	//オブジェクト行列を更新
	UpdateTransform();

	//モデルアニメーション更新
	model->UpdateAnimation(elapsedTime);

	//モデル行列更新
	model->UpdateTransform(tranceform);

	//速力処理更新
	UPdateVelocity(elapsedTime);
	UpdateInvincTimer(elapsedTime);
}

void Boxes::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);

	DrawDebugPrimitive();
}

//デバッグプリミティブ描画
void Boxes::DrawDebugPrimitive()
{
	//基底クラスのデバッグプリミティブ描画
	Box::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

}

