#include "StageMain.h"
#include <imgui.h>
#include "Input/Input.h"


//�R���X�g���N�^
StageMain::StageMain()
{
	//�X�e�[�W���f����ǂݍ���
	model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
}
	StageMain::~StageMain()
	{
		//�X�e�[�W���f����j��
		delete model;
	}

	//�X�V����
	void StageMain::Update(float elapsedTime)
	{
		//WASD�L�[�ŉ�]
		UpdateStageRotate(elapsedTime);

		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

		DirectX::XMStoreFloat4x4(&stage_transform, R);
		//�X�e�[�W�s��̍X�V����
		model->UpdateTransform(stage_transform);
	}

	//�`�揈��
	void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
	{
		//�V�F�[�_�[�Ƀ��f���`������Ă��炤
		shader->Draw(dc, model);
		model->UpdateTransform(stage_transform);
	}

	


	//���C�L���X�g
	bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
	{
		return Collision::IntersecRayVsModel(start, end, model, hit);
	}


	//�X�e�[�W�̉�]�X�V����
	void StageMain::UpdateStageRotate(float elapsedTime)
	{
		//W
		GamePad& gamepad = Input::Instance().GetGamePad();
		
		if (gamepad.GetButton() & GamePad::KEY_W) // ���]
		{
			isRotating = true;
			rotation.x -= rotateSpeed * elapsedTime;
		}
		//A
		if (gamepad.GetButton() & GamePad::KEY_A) // ����]
		{
			isRotating = true;
			rotation.z -= rotateSpeed * elapsedTime;
		}
		//S
		if (gamepad.GetButton() & GamePad::KEY_S) // ����]
		{
			isRotating = true;
			rotation.x += rotateSpeed * elapsedTime;
		}
		//D
		if (gamepad.GetButton() & GamePad::KEY_D) // �E��]
		{
			isRotating = true;
			rotation.z += rotateSpeed * elapsedTime;
		}
	}
