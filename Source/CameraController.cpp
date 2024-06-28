#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
void CameraController::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	//カメラの回転速度
	float speed = rollSpeed * elapsedTime;

	//  スティックの入力値に合わせてX軸とY軸を回転
	angle.x += ay * speed; // X軸の回転
	angle.y += ax * speed; // Y軸の回転

	// X軸のカメラ回転を制限
	const float maxPitch = DirectX::XM_PI / 4.0f;
	const float minPitch = -maxPitch;

	//  ピッチが最大値を超えた場合は、最大値に制限
	if (angle.x > maxPitch)
	{
		angle.x = maxPitch;
	}
	//  ピッチが最小値を下回った場合は、最小値に制限
	else if (angle.x < minPitch)
	{
		angle.x = minPitch;
	}

	// Y軸の回転軸を-3.14～3.14に収まるようにする
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	//カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//注視点から後ろのベクトル方向に一定距離離れたカメラ視点を求める
	DirectX::XMFLOAT3 eye;
	
	eye.x = this->target.x - front.x * this->range;
	eye.y = this->target.y - front.y * this->range;
	eye.z = this->target.z - front.z * this->range;
	//カメラの視点と注視点を設定
	Camera::Instance().SetLoolAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}
