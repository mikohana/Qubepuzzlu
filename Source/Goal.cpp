#include<imgui.h>
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Boxes.h"
#include "BoxManager.h"
#include "Goal.h"
#include "Collision.h"

#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"

//�C���X�^���X�ϐ�
static Goal* instance = nullptr;

//�C���X�^���X�擾
Goal& Goal::Instance()
{
	return *instance;
}


Goal::Goal()
{
	//�C���X�^���X�|�C���^�ݒ�
	instance = this;

	//model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
	model = new Model("Data/Model/Star/Star.mdl");

	//�q�b�g�G�t�F�N�g�ǂݍ���
	hitEffect = new Effect("Data/Effect/Hit.efk");

	//���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

}
Goal::~Goal()
{
	delete hitEffect;

	delete model;
}


void Goal::Update(float elapedTime)
{
	
	//�v���C���[�ƃG�l�~�[�̏Փ˔���
	void CollisionPlayerVsGoal();

	UpdateTransform();

	//���f���A�j���[�V�����X�V����
	model->UpdateAnimation(elapedTime);

	//���f���s��X�V
	model->UpdateTransform(tranceform);

}


void Goal::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	//�S�[���`��
	shader->Draw(dc, model);
}


//�v���C���[�ƃS�[���̓����蔻��
void Goal::CollisionPlayerVsGoal()
{
	// �v���C���[�̃C���X�^���X���擾
	Player& player = Player::Instance();

	// �v���C���[�ƃS�[���̏Փ˔���
	DirectX::XMFLOAT3 outPosition;
	if (Collision::IntersecCylinderVsCylinder(
		player.GetPosition(), 2.0f, player.GetHeight(),
		this->GetPosition(), 2.0f, this->GetHeight(),
		outPosition
	))
	{
		// ����������
		//�S�[���̈ʒu���X�V����
		this->SetPosition(outPosition);
	}
}