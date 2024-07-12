#include "Boxes.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "Collision.h"

Boxes::Boxes()
{
	model = new Model("Data/Model/Cube/Cube.mdl");

	//�X�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

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
	

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	//���f���A�j���[�V�����X�V
	model->UpdateAnimation(elapsedTime);

	//���f���s��X�V
	model->UpdateTransform(tranceform);

	//���͏����X�V
	UPdateVelocity(elapsedTime);
	UpdateInvincTimer(elapsedTime);
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

