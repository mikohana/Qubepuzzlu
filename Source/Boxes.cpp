#include "Boxes.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "Collision.h"
#include "StageManager.h"
#include "StageMain.h"
#include "Camera.h"

Boxes::Boxes(BoxColor color) : color(color)
{
	switch (color)
	{
	case BoxColor::RED:
		models[0].push_back(new Model("Data/Model/Boxes/RedBox.mdl"));
		break;
	case BoxColor::GREEN:
		models[1].push_back(new Model("Data/Model/Boxes/GreenBox.mdl"));
		break;
	case BoxColor::BLUE:
		models[2].push_back(new Model("Data/Model/Boxes/BlueBox.mdl"));
		break;
	case BoxColor::PLAYER:
		models[3].push_back(new Model("Data/Model/Dise/Dise/mdl"));
	}

	//スケーリング
	scale.x = scale.y = scale.z = 1.0f;

	//幅、高さ
	radius = 0.5f;
	height = 1.0f;
}

Boxes::~Boxes()
{
	for (int i = 0; i < 3; i++)
		for (auto model : models[i])
			delete model;
}

void Boxes::Update(float elapsedTime)
{
	//カメラの回転角度を同期
    Camera* camera = nullptr;
	

	// 親ステージの回転角度を同期
	StageMain* stageMain = (StageMain*)StageManager::Instance().GetStage(0);

	angle = stageMain->rotation;



	//速力処理更新
	UPdateVelocity(elapsedTime);
	UpdateInvincTimer(elapsedTime);

	//オブジェクト行列を更新
	UpdateTransform();

	//モデルアニメーション更新
	for (int i = 0; i < 3; i++)
		for (auto model : models[i])
			model->UpdateAnimation(elapsedTime);

	//モデル行列更新
	for (int i = 0; i < 3; i++)
		for (auto model : models[i])
			model->UpdateTransform(tranceform);

}

void Boxes::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (int i = 0; i < 3; i++)
	{
		//色を設定
		DirectX::XMFLOAT4 color;
		switch (static_cast<BoxColor>(i))
		{
		case BoxColor::RED:
			color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case BoxColor::GREEN:
			color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			break;
		case BoxColor::BLUE:
			color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		}
		//シェーダーに色を設定
		//shader->SetColor(color);

		//モデルを描画
		for (auto model : models[i])
		{
			shader->Draw(dc, model);
		}
	}
		

	DrawDebugPrimitive();
}

//デバッグプリミティブ描画
void Boxes::DrawDebugPrimitive()
{
	//基底クラスのデバッグプリミティブ描画
	Box::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	
}
