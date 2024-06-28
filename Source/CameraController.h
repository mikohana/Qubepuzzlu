#pragma once

#include <DirectXMath.h>

//カメラコントローラー
class CameraController
{
public:
	CameraController() {};
	~CameraController() {};

	void Update(float elapedTime);

	//ターゲット位置設定
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

private:
	DirectX::XMFLOAT3 target = { 0,0,0 };
	DirectX::XMFLOAT3 angle =  { 0,0,0 };
	float             rollSpeed = DirectX::XMConvertToRadians(90);
	float             range = 10.0f;

	float maxAngle = DirectX::XMConvertToRadians(45);
	float minAngle = DirectX::XMConvertToRadians(-45);
};