#include<imgui.h>
#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#if true

CameraController::CameraController()
{
	angle.x = DirectX::XM_PI / 2.0f;
	angle.y = 0.0f;
	angle.z = 0.0f;

}
void CameraController::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	//カメラの回転処理
	if (gamepad.GetButton() & GamePad::KEY_W) // 上回転
	{
		angle.x += elapsedTime / 10;

	}
	else if (gamepad.GetButton() & GamePad::KEY_A) // 左回転
	{
		angle.z -= elapsedTime / 10;

	}
	else if (gamepad.GetButton() & GamePad::KEY_S) // 下回転
	{
		angle.x -= elapsedTime / 10;
	}
	else if (gamepad.GetButton() & GamePad::KEY_D) // 右回転
	{
		angle.z += elapsedTime / 10;
	}
	else
	{
		angle.x = DirectX::XM_PI / 2.0f;
		angle.y = DirectX::XM_PI / 2.0f;
		angle.z = DirectX::XM_PI / 2.0f;
	
	}

	// X軸のカメラ回転を制限
	const float maxPitch = DirectX::XM_PI;
	const float minPitch = 0.0f;

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
	// Z軸のカメラ回転を制限
	const float maxRoll = DirectX::XM_PI; // 180度
	const float minRoll = 0.0f; // 0度

	// ロールが最大値を超えた場合は、最大値に制限
	if (angle.z > maxRoll)
	{
		angle.z = maxRoll;
	}
	// ロールが最小値を下回った場合は、最小値に制限
	else if (angle.z < minRoll)
	{
		angle.z = minRoll;
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
#endif // false

#if false

CameraController::CameraController()
{
	// クオータニオンの初期化
	quaternion = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XM_PI / 2.0f, 0.0f, 0.0f);
}
void CameraController::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	DirectX::XMVECTOR rotationAxis = {};
	float rotationAngle = elapsedTime / 10;

	// カメラの回転処理
	if (gamepad.GetButton() & GamePad::KEY_W) // 上回転
	{
		rotationAxis = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else if (gamepad.GetButton() & GamePad::KEY_A) // 左回転
	{
		rotationAxis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	}
	else if (gamepad.GetButton() & GamePad::KEY_S) // 下回転
	{
		rotationAxis = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	}
	else if (gamepad.GetButton() & GamePad::KEY_D) // 右回転
	{
		rotationAxis = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	}
	else
	{
		quaternion = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XM_PI / 2.0f, 0.0f, 0.0f);
		rotationAxis = DirectX::XMVectorSet(0.0f, 0.0001f, 0.0f,0.0f);
		rotationAngle = 0.0f;
		return;
	}

	// 回転軸と角度からクオータニオンを計算
	DirectX::XMVECTOR rotationQuat = DirectX::XMQuaternionRotationAxis(rotationAxis, rotationAngle);

	// 現在のクオータニオンに新たな回転を適用
	quaternion = DirectX::XMQuaternionMultiply(quaternion, rotationQuat);

	// クオータニオンから回転行列を計算
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(quaternion);

	// 回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// 注視点から後ろのベクトル方向に一定距離離れたカメラ視点を求める
	DirectX::XMFLOAT3 eye;
	eye.x = this->target.x - front.x * this->range;
	eye.y = this->target.y - front.y * this->range;
	eye.z = this->target.z - front.z * this->range;

	// カメラの視点と注視点を設定
	Camera::Instance().SetLoolAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

#endif // false

#if false

CameraController::CameraController()
{
	// クオータニオンの初期化
	DirectX::XMVECTOR quaternionVec = DirectX::XMQuaternionRotationRollPitchYaw(0.0f, -1.0f, 0.0f);
	DirectX::XMStoreFloat4(&quaternion, quaternionVec);
}
void CameraController::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(1.0f, 0.0f, 0.000001f, 0.0f);
	float rotationAngle = elapsedTime * 2.0f;

	// カメラの回転処理
	if (gamepad.GetButton() & GamePad::KEY_W) // 上回転
	{
		rotationAxis = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else if (gamepad.GetButton() & GamePad::KEY_A) // 左回転
	{
		rotationAxis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	}
	else if (gamepad.GetButton() & GamePad::KEY_S) // 下回転
	{
		rotationAxis = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	}
	else if (gamepad.GetButton() & GamePad::KEY_D) // 右回転
	{
		rotationAxis = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	}
	else
	{
		quaternion = DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
		rotationAxis = DirectX::XMVectorSet(0.0f, 0.0f, 0.00001f, 0.0f);
		rotationAngle = 0.0f;
	}

	// 回転軸と角度からクオータニオンを計算
	DirectX::XMVECTOR rotationQuat = DirectX::XMQuaternionRotationAxis(rotationAxis, rotationAngle);

#if true
	// 現在のクオータニオンに新たな回転を適用
	DirectX::XMVECTOR quaternionVec = DirectX::XMLoadFloat4(&quaternion);
	quaternionVec = DirectX::XMQuaternionMultiply(quaternionVec, rotationQuat);

	// クオータニオンから回転行列を計算
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(quaternionVec);
#else
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(rotationQuat);

#endif


	// 回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// 注視点から後ろのベクトル方向に一定距離離れたカメラ視点を求める
	DirectX::XMFLOAT3 eye;
	eye.x = this->target.x - front.x * this->range;
	eye.y = this->target.y - front.y * this->range;
	eye.z = this->target.z - front.z * this->range;

	// カメラの視点と注視点を設定
	Camera::Instance().SetLoolAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));

#if true
	// quaternionVecからquaternionを更新
	DirectX::XMStoreFloat4(&quaternion, quaternionVec);
#else
	DirectX::XMStoreFloat4(&quaternion, rotationQuat);
#endif
}

#endif


void CameraController::DrawDebugGuI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_None)) {

		if (ImGui::CollapsingHeader("CameraController", ImGuiTreeNodeFlags_DefaultOpen)) {
			CameraController& cameraController = CameraController::Instance();

			ImGui::InputFloat3("Position", &cameraController.eye.x);
			ImGui::InputFloat3("Target", &cameraController.target.x);

			//// DirectX::XMFLOAT3の各成分を個別に扱う
			//ImGui::InputFloat3("Angle", &cameraController.GetAngle().x);
			//ImGui::InputFloat3("Angle", &cameraController.GetAngle().y);
			//ImGui::InputFloat3("Angle", &cameraController.GetAngle().z);
		}
	}
	ImGui::End();
}
