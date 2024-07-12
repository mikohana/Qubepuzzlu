#include<imgui.h>
#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

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
		angle.z += elapsedTime;
	}
	else if (gamepad.GetButton() & GamePad::KEY_A) // 左回転
	{

		angle.x -= elapsedTime;
	}
	else if (gamepad.GetButton() & GamePad::KEY_S) // 下回転
	{
		angle.z -= elapsedTime;
	}
	else if (gamepad.GetButton() & GamePad::KEY_D) // 右回転
	{
		angle.x += elapsedTime;
	}
	else
	{

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
