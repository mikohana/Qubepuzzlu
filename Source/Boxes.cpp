#include "Boxes.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "Collision.h"
#include "StageManager.h"
#include "StageMain.h"
#include "Camera.h"

Boxes::Boxes()
{
	model = new Model("Data/Model/Cube/Cube.mdl");
	//models[0].push_back(new Model("Data/Model/Boxes/RedBox.mdl"));
	

	//�X�P�[�����O
	scale.x = scale.y = scale.z = 2.0f;

	//���A����
	radius = 0.5f;
	height = 1.0f;
}

Boxes::~Boxes()
{
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
	model->UpdateAnimation(elapsedTime);

	//���f���s��X�V
	model->UpdateTransform(tranceform);

}

void Boxes::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);

	DrawDebugPrimitive();
}




//�f�o�b�O�v���~�e�B�u�`��
void Boxes::DrawDebugPrimitive()
{
	//���N���X�̃f�o�b�O�v���~�e�B�u�`��
	Box::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	
}



