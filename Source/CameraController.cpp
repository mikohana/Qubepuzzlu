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

	//�J�����̉�]����
	if (gamepad.GetButton() & GamePad::KEY_W) // ���]
	{
		angle.z += elapsedTime;
	}
	else if (gamepad.GetButton() & GamePad::KEY_A) // ����]
	{

		angle.x -= elapsedTime;
	}
	else if (gamepad.GetButton() & GamePad::KEY_S) // ����]
	{
		angle.z -= elapsedTime;
	}
	else if (gamepad.GetButton() & GamePad::KEY_D) // �E��]
	{
		angle.x += elapsedTime;
	}
	else
	{

	}

	// X���̃J������]�𐧌�
	const float maxPitch = DirectX::XM_PI;
	const float minPitch = 0.0f;

	//  �s�b�`���ő�l�𒴂����ꍇ�́A�ő�l�ɐ���
	if (angle.x > maxPitch)
	{
		angle.x = maxPitch;
	}
	//  �s�b�`���ŏ��l����������ꍇ�́A�ŏ��l�ɐ���
	else if (angle.x < minPitch)
	{
		angle.x = minPitch;
	}
	// Z���̃J������]�𐧌�
	const float maxRoll = DirectX::XM_PI; // 180�x
	const float minRoll = 0.0f; // 0�x

	// ���[�����ő�l�𒴂����ꍇ�́A�ő�l�ɐ���
	if (angle.z > maxRoll)
	{
		angle.z = maxRoll;
	}
	// ���[�����ŏ��l����������ꍇ�́A�ŏ��l�ɐ���
	else if (angle.z < minRoll)
	{
		angle.z = minRoll;
	}
			//�J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//�����_������̃x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;

	eye.x = this->target.x - front.x * this->range;
	eye.y = this->target.y - front.y * this->range;
	eye.z = this->target.z - front.z * this->range;
	//�J�����̎��_�ƒ����_��ݒ�
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

			//// DirectX::XMFLOAT3�̊e�������ʂɈ���
			//ImGui::InputFloat3("Angle", &cameraController.GetAngle().x);
			//ImGui::InputFloat3("Angle", &cameraController.GetAngle().y);
			//ImGui::InputFloat3("Angle", &cameraController.GetAngle().z);
		}
	}
	ImGui::End();
}
