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
	position = DirectX::XMFLOAT3(5.0f, 0.0f, -3.0f);
	height = 1.0f;
	radius = 0.5f;

}
Goal::~Goal()
{
	delete hitEffect;

	delete model;
}


void Goal::Update(float elapedTime)
{

	//�{�b�N�X�ƃS�[���̔���
	//CollisionBoxesVsGoal();

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
//
////���ׂẴu���b�N�ƃS�[���̓����蔻��
//void Goal::CollisionBoxesVsGoal()
//{
//	// BoxManager���炷�ׂẴ{�b�N�X���擾
//	BoxManager& boxManager = BoxManager::Instance();
//
//	std::vector<Boxes*> allBoxes = boxManager.GetAllBoxes();
//
//	// ���ׂẴ{�b�N�X�ƃS�[���̏Փ˔���
//	for (Boxes* box : allBoxes)
//	{
//		// �{�b�N�X��Player�ł��邩�m�F
//		if (box->GetColor() == BoxColor::PLAYER)
//		{
//			DirectX::XMFLOAT3 outPosition;
//			if (Collision::IntersecCylinderVsCylinder(
//				box->GetPosition(), 2.0f, box->GetHeight(),
//				this->GetPosition(), 2.0f, this->GetHeight(),
//				outPosition
//			))
//			{
//				// Player�̃{�b�N�X�ƃS�[�����Փ˂�����^�C�g���V�[���ɐ؂�ւ�
//				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
//				break;  // �Փ˂��m�F�ł����烋�[�v�𔲂���
//			}
//		}
//	}
//}