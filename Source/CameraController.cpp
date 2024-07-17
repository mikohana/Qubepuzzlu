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
	//�J�����̉�]����
	if (gamepad.GetButton() & GamePad::KEY_W) // ���]
	{
		angle.x += elapsedTime / 10;

	}
	else if (gamepad.GetButton() & GamePad::KEY_A) // ����]
	{
		angle.z -= elapsedTime / 10;

	}
	else if (gamepad.GetButton() & GamePad::KEY_S) // ����]
	{
		angle.x -= elapsedTime / 10;
	}
	else if (gamepad.GetButton() & GamePad::KEY_D) // �E��]
	{
		angle.z += elapsedTime / 10;
	}
	else
	{
		angle.x = DirectX::XM_PI / 2.0f;
		angle.y = DirectX::XM_PI / 2.0f;
		angle.z = DirectX::XM_PI / 2.0f;
	
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
#endif // false

#if false

CameraController::CameraController()
{
	// �N�I�[�^�j�I���̏�����
	quaternion = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XM_PI / 2.0f, 0.0f, 0.0f);
}
void CameraController::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	DirectX::XMVECTOR rotationAxis = {};
	float rotationAngle = elapsedTime / 10;

	// �J�����̉�]����
	if (gamepad.GetButton() & GamePad::KEY_W) // ���]
	{
		rotationAxis = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else if (gamepad.GetButton() & GamePad::KEY_A) // ����]
	{
		rotationAxis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	}
	else if (gamepad.GetButton() & GamePad::KEY_S) // ����]
	{
		rotationAxis = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	}
	else if (gamepad.GetButton() & GamePad::KEY_D) // �E��]
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

	// ��]���Ɗp�x����N�I�[�^�j�I�����v�Z
	DirectX::XMVECTOR rotationQuat = DirectX::XMQuaternionRotationAxis(rotationAxis, rotationAngle);

	// ���݂̃N�I�[�^�j�I���ɐV���ȉ�]��K�p
	quaternion = DirectX::XMQuaternionMultiply(quaternion, rotationQuat);

	// �N�I�[�^�j�I�������]�s����v�Z
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(quaternion);

	// ��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// �����_������̃x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = this->target.x - front.x * this->range;
	eye.y = this->target.y - front.y * this->range;
	eye.z = this->target.z - front.z * this->range;

	// �J�����̎��_�ƒ����_��ݒ�
	Camera::Instance().SetLoolAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

#endif // false

#if false

CameraController::CameraController()
{
	// �N�I�[�^�j�I���̏�����
	DirectX::XMVECTOR quaternionVec = DirectX::XMQuaternionRotationRollPitchYaw(0.0f, -1.0f, 0.0f);
	DirectX::XMStoreFloat4(&quaternion, quaternionVec);
}
void CameraController::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(1.0f, 0.0f, 0.000001f, 0.0f);
	float rotationAngle = elapsedTime * 2.0f;

	// �J�����̉�]����
	if (gamepad.GetButton() & GamePad::KEY_W) // ���]
	{
		rotationAxis = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else if (gamepad.GetButton() & GamePad::KEY_A) // ����]
	{
		rotationAxis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	}
	else if (gamepad.GetButton() & GamePad::KEY_S) // ����]
	{
		rotationAxis = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	}
	else if (gamepad.GetButton() & GamePad::KEY_D) // �E��]
	{
		rotationAxis = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	}
	else
	{
		quaternion = DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
		rotationAxis = DirectX::XMVectorSet(0.0f, 0.0f, 0.00001f, 0.0f);
		rotationAngle = 0.0f;
	}

	// ��]���Ɗp�x����N�I�[�^�j�I�����v�Z
	DirectX::XMVECTOR rotationQuat = DirectX::XMQuaternionRotationAxis(rotationAxis, rotationAngle);

#if true
	// ���݂̃N�I�[�^�j�I���ɐV���ȉ�]��K�p
	DirectX::XMVECTOR quaternionVec = DirectX::XMLoadFloat4(&quaternion);
	quaternionVec = DirectX::XMQuaternionMultiply(quaternionVec, rotationQuat);

	// �N�I�[�^�j�I�������]�s����v�Z
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(quaternionVec);
#else
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(rotationQuat);

#endif


	// ��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// �����_������̃x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = this->target.x - front.x * this->range;
	eye.y = this->target.y - front.y * this->range;
	eye.z = this->target.z - front.z * this->range;

	// �J�����̎��_�ƒ����_��ݒ�
	Camera::Instance().SetLoolAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));

#if true
	// quaternionVec����quaternion���X�V
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

			//// DirectX::XMFLOAT3�̊e�������ʂɈ���
			//ImGui::InputFloat3("Angle", &cameraController.GetAngle().x);
			//ImGui::InputFloat3("Angle", &cameraController.GetAngle().y);
			//ImGui::InputFloat3("Angle", &cameraController.GetAngle().z);
		}
	}
	ImGui::End();
}
