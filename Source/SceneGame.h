#pragma once
#include "Stage.h"
#include "Player.h"
#include "Scene.h"
#include "Goal.h"
#include "Graphics/Sprite.h"
#include "CameraController.h"
#include "Graphics/SkyBox.h"
//ビューポートの範囲の構造体
struct  Viewport
{
	float x;
	float y;
	float Width;
	float Height;
	float MinZ;
	float MaxZ;
};


// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override{};

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize();

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

	
private:
	//エネミーHPゲージ描画
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	//ワールド座標からスクリーン座標への変換を行う関数
	DirectX::XMFLOAT2 WorldToScreen(const DirectX::XMFLOAT3& worldPos, const Viewport& viewport, const DirectX::FXMMATRIX projection, const DirectX::FXMMATRIX& view, const DirectX::FXMMATRIX world)
	{
		//スクリーン座標を計算
		DirectX::XMVECTOR screenPosVec = DirectX::XMVector3Project(
			DirectX::XMLoadFloat3(&worldPos),
			viewport.x,
			viewport.y,
			viewport.Width,
			viewport.Height,
			viewport.MinZ,
			viewport.MaxZ,
			projection,
			view,
			world
		);
		
		//XMVECTORからXMFLOAT2に変換
		DirectX::XMFLOAT2 screenPos;
		screenPos.x = DirectX::XMVectorGetX(screenPosVec);
		screenPos.y = DirectX::XMVectorGetY(screenPosVec);
		return screenPos;
	};

	//スクリーン座標からワールド座標への変換させる関数
	DirectX::XMFLOAT3 ScreenToWorld(float x,float y,float z,const Viewport& viewport,const DirectX::FXMMATRIX projectile,const DirectX::FXMMATRIX view,const DirectX::FXMMATRIX world)
	{
		//ワールド座標を計算
		DirectX::XMVECTOR worldPosVec = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z)),
			viewport.x,
			viewport.y,
			viewport.Width,
			viewport.Height,
			viewport.MinZ,
			viewport.MaxZ,
			projectile,
			view,
			world
		);

		//XMVECTORからXMFLOAT3に変換
		DirectX::XMFLOAT3 worldPos;
		DirectX::XMStoreFloat3(&worldPos, worldPosVec);

		return worldPos;
	};

private:
	SkyBox* skybox = nullptr;

	Stage* stage = nullptr;

	Player* player = nullptr;

	Sprite* gauge = nullptr;

	Goal* goal = nullptr;

	CameraController* cameraController = nullptr;

	int clickCount = 0;
	int textureWidth; // テクスチャの幅
	int gaugeWidth;   // 現在のゲージの幅
	

};
