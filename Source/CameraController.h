#pragma once

#include <DirectXMath.h>

//カメラコントローラー
class CameraController
{
public:
	CameraController();
	~CameraController() {};
	//インスタンスの取得
	static CameraController& Instance()
	{
		static CameraController instance;
		return instance;
	}

	void Update(float elapedTime);

	//デバッグ用GUI描画処理
	void DrawDebugGuI();

	//ターゲット位置設定
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

	//回転率取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	//注視点取得
	const DirectX::XMFLOAT3& GetEye() const { return eye; }
private:
	DirectX::XMFLOAT3 target = { 0,0,0 };
	DirectX::XMFLOAT3 angle =  { 0,0,0 };
	DirectX::XMFLOAT3 eye = { 0,0,0 };
	float             rollSpeed = DirectX::XMConvertToRadians(90);
	float             range = 15.0f;

	float rotateSpeed = 1.0f;

	float maxAngle = DirectX::XMConvertToRadians(45);
	float minAngle = DirectX::XMConvertToRadians(-45);
};