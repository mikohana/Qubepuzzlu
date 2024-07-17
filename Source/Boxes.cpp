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
		models[3].push_back(new Model("Data/Model/Dise/Dise.mdl"));
		break;
	case BoxColor::GOAL:
		models[4].push_back(new Model("Data/Model/Star/Star.mdl"));
		break;
	case BoxColor::NONE:
		nullptr;
	}

	if (BoxColor::GOAL == color)
	{
		scale = { 0.01f,0.01f,0.01f };
	}
	else
	{
		//�X�P�[�����O
		scale.x = scale.y = scale.z = 1.0f;
	}
	

	//���A����
	radius = 0.5f;
	height = 1.0f;
}

Boxes::~Boxes()
{
	for (int i = 0; i < 5; i++)
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




#if false
	//���͏����X�V
	if (color != BoxColor::GREEN) {
		UPdateVelocity(elapsedTime);
}
#endif // false
	if (BoxColor::GOAL != color)
	{
		//���͏����X�V
		UPdateVelocity(elapsedTime);
	}
	


	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	//���f���A�j���[�V�����X�V
	for (int i = 0; i < 5; i++)
		for (auto model : models[i])
			model->UpdateAnimation(elapsedTime);

	//���f���s��X�V
	for (int i = 0; i < 5; i++)
		for (auto model : models[i])
			model->UpdateTransform(tranceform);

}

void Boxes::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (int i = 0; i < 5; i++)
	{
#if false
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
		case BoxColor::PLAYER:
			color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			//�V�F�[�_�[�ɐF��ݒ�
			//shader->SetColor(color);

		}
#endif // false

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
