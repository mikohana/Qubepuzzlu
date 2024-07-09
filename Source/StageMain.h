#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

//ステージ
class StageMain : public Stage
{
public:
	StageMain();
	~StageMain() override;

	//更新処理
	void Update(float elapedTime) override;
	//描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	
	//レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;
	
	//ステージの回転更新処理
	void UpdateStageRotate(float elapsedTime);
	


public:
	Model* model = nullptr;
	float rotateSpeed = 0.3f;   //回転速度(度/秒)
	float totalRotation = 0.0f;
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	bool isRotating = false;
	DirectX::XMFLOAT3 rotation = { 0,0,0 };


	DirectX::XMFLOAT4X4 stage_transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
};