#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

//移動床ステージ
class StageMoveFloor : public Stage
{
public:
	StageMoveFloor();
	~StageMoveFloor();

	//更新処理
	void Update(float elapsedTile)override;

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;

	//スタート位置設定
	void SetStartPoint(const DirectX::XMFLOAT3& start) {this->start}

private:
	Model* model = nullptr;
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 angle = { 0,0,0 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	//変換行列44
	DirectX::XMFLOAT4X4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 
	};
	DirectX::XMFLOAT3 torque = { 0,0,0 };
	DirectX::XMFLOAT3 start =  { 0,0,0 };
	DirectX::XMFLOAT3 goal =   { 0,0,0 };

	float moveSpeed = 2.0f;
	float moveRate =  0.0f;
};