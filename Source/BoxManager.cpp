#include "BoxManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"
#include "Player.h"


#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"


void BoxManager::Update(float elapsedTime)
{
	for (Box* box : box)
	{
		box->Update(elapsedTime);
	}

	for (Box* box : remove)
	{
		std::vector<Box*>::iterator it = std::find(this->box.begin(), this->box.end(), box);
		if (it != this->box.end())
		{
			this->box.erase(it);
		}
		//�{�b�N�X��j��
		delete box;
	}
	//�j�����X�g���N���A
	remove.clear();

	//�{�b�N�X���m�̏Փˏ���
	CollisionBoxVsBox();

	//�{�b�N�X�ƃv���C���[�̓����蔻��
	CollisionBoxVsPlayer();

	//Player�ƃS�[���̓����蔻��
	CollisionBoxesVsGoal();

}

void BoxManager::Render(ID3D11DeviceContext* constext, Shader* shader)
{
	for (Box* box : box)
	{
		box->Render(constext, shader);
	}
}

//�{�b�N�X�o�^
void BoxManager::Register(Box* box)
{
	this->box.emplace_back(box);
}

//�{�b�N�X�S�폜
void BoxManager::Clear()
{
	for (Box* enemy : box)
	{
		delete enemy;
	}
	box.clear();
}

void BoxManager::DrawDebugPrimitive()
{
	DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();

	debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4{ 0,0,0,1 });
}

void BoxManager::Remove(Box* box)
{
	//�j�����X�g�ɒǉ�
	remove.insert(box);
}


//�{�b�N�X���m�̏Փˏ���
void BoxManager::CollisionBoxVsBox()
{
	size_t boxCount = box.size();
	for (int i = 0; i < boxCount; ++i)
	{
		Boxes* boxA = dynamic_cast<Boxes*>(box.at(i));
		for (int j = i + 1; j < boxCount; ++j)
		{
			Boxes* boxB = dynamic_cast<Boxes*>(box.at(j));

			DirectX::XMFLOAT3 outPosition;

			if (Collision::IntersecCylinderVsCylinder(
				boxA->GetPosition(), 1.0f, boxA->GetHeight(),
				boxB->GetPosition(), 1.0f, boxB->GetHeight(),
				outPosition
			))
			{
				if (boxA->GetColor() == boxB->GetColor())
				{
					//�����u���b�N�̐F���m���Փ˂�����A�������폜
					Remove(boxA);
					Remove(boxB);
				}
				else
				{
					//�قȂ�F�̃{�b�N�X���Փ˂����ꍇ�AA��B�������o��
					if(boxB->GetColor() != BoxColor::GOAL)
					boxB->SetPosition(outPosition);
				}
			}
		}
	}
}

//�v���C���[�ƃu���b�N�̓����蔻��
void BoxManager::CollisionBoxVsPlayer()
{
	size_t boxCount = box.size();
	for (int i = 0; i < boxCount; ++i)
	{
		Boxes* boxA = dynamic_cast<Boxes*>(box.at(i));
		if (boxA->GetColor() == BoxColor::PLAYER)
		{
			for (int j = i + 1; j < boxCount; ++j)
			{
				Boxes* boxB = dynamic_cast<Boxes*>(box.at(j));
				if (boxB->GetColor() != BoxColor::PLAYER)
				{
					DirectX::XMFLOAT3 outPosition;
					if (Collision::IntersecCylinderVsCylinder(
						boxA->GetPosition(), 1.0f, boxA->GetHeight(),
						boxB->GetPosition(), 1.0f, boxB->GetHeight(),
						outPosition
					))
					{
						boxB->SetPosition(outPosition);
					}
				}
			}
		}
	}
}

//�v���C���[�ƃS�[���̓����蔻��
void BoxManager::CollisionBoxesVsGoal()
{
	size_t boxCount = box.size();
	for (int i = 0; i < boxCount; ++i)
	{
		Boxes* boxA = dynamic_cast<Boxes*>(box.at(i));
		if (boxA->GetColor() == BoxColor::PLAYER)
		{
			for (int j = i + 1; j < boxCount; ++j)
			{
				Boxes* boxB = dynamic_cast<Boxes*>(box.at(j));
				if (boxB->GetColor() == BoxColor::GOAL)
				{
					DirectX::XMFLOAT3 outPosition;
					if (Collision::IntersecCylinderVsCylinder(
						boxA->GetPosition(), 1.0f, boxA->GetHeight(),
						boxB->GetPosition(), 1.0f, boxB->GetHeight(),
						outPosition
					))
					{
						// Player�̃{�b�N�X��Goal���Փ˂�����^�C�g���V�[���ɐ؂�ւ�
						SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
						return;
					}
				}
			}
		}
	}
}
