#include "Boxes.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "Collision.h"
#include "StageManager.h"
#include "StageMain.h"
#include "Camera.h"

Boxes::Boxes(BoxColor color) : color(color)
{
	switch (color)
	{
	case BoxColor::RED:
		models[0].push_back(new Model("Data/Model/Boxes/RedBox.mdl"));
		break;
	case BoxColor::GREEN:
		models[1].push_back(new Model("Data/Model/Boxes/GreenBox.mdl"));
		break;
	case BoxColor::BLUE:
		models[2].push_back(new Model("Data/Model/Boxes/BlueBox.mdl"));
		break;
	case BoxColor::PLAYER:
		models[3].push_back(new Model("Data/Model/Dise/Dise/mdl"));
	}

	//�X�P�[�����O
	scale.x = scale.y = scale.z = 1.0f;

	//���A����
	radius = 0.5f;
	height = 1.0f;
}

Boxes::~Boxes()
{
	for (int i = 0; i < 3; i++)
		for (auto model : models[i])
			delete model;
}

void Boxes::Update(float elapsedTime)
{
	//�J�����̉�]�p�x�𓯊�
    Camera* camera = nullptr;
	

	// �e�X�e�[�W�̉�]�p�x�𓯊�
	StageMain* stageMain = (StageMain*)StageManager::Instance().GetStage(0);

	angle = stageMain->rotation;



	//���͏����X�V
	UPdateVelocity(elapsedTime);
	UpdateInvincTimer(elapsedTime);

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	//���f���A�j���[�V�����X�V
	for (int i = 0; i < 3; i++)
		for (auto model : models[i])
			model->UpdateAnimation(elapsedTime);

	//���f���s��X�V
	for (int i = 0; i < 3; i++)
		for (auto model : models[i])
			model->UpdateTransform(tranceform);

}

void Boxes::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (int i = 0; i < 3; i++)
	{
		//�F��ݒ�
		DirectX::XMFLOAT4 color;
		switch (static_cast<BoxColor>(i))
		{
		case BoxColor::RED:
			color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case BoxColor::GREEN:
			color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			break;
		case BoxColor::BLUE:
			color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		}
		//�V�F�[�_�[�ɐF��ݒ�
		//shader->SetColor(color);

		//���f����`��
		for (auto model : models[i])
		{
			shader->Draw(dc, model);
		}
	}
		

	DrawDebugPrimitive();
}

//�f�o�b�O�v���~�e�B�u�`��
void Boxes::DrawDebugPrimitive()
{
	//���N���X�̃f�o�b�O�v���~�e�B�u�`��
	Box::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	
}
