#include "BoxManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"
#include "Player.h"

void BoxManager::Update(float elapsedTime)
{
	for (Box* box : boxes)
	{
		box->Update(elapsedTime);
	}

	for (Box* box : remove)
	{
		std::vector<Box*>::iterator it = std::find(boxes.begin(), boxes.end(), box);
		if (it != boxes.end())
		{
			boxes.erase(it);
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
}

void BoxManager::Render(ID3D11DeviceContext* constext, Shader* shader)
{
	for (Box* box : boxes)
	{
		box->Render(constext, shader);
	}
}

//�{�b�N�X�o�^
void BoxManager::Register(Box* box)
{
	boxes.emplace_back(box);
}

//�{�b�N�X�S�폜
void BoxManager::Clear()
{
	for (Box* enemy : boxes)
	{
		delete enemy;
	}
	boxes.clear();
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
//
//
////�~���Ɖ~��
//void BoxManager::CollisionBoxVsBox()
//{
//	size_t boxCount = boxes.size();
//	for (int i = 0; i < boxCount; ++i)
//	{
//		Box* boxA = boxes.at(i);
//		for (int j = i + 1; j < boxCount; ++j)
//		{
//			Box* boxB = boxes.at(j);
//
//			DirectX::XMFLOAT3 outPosition;
//			
//			if (Collision::IntersecCylinderVsCylinder(
//				boxA->GetPosition(), 2.0f, boxA->GetHeight(), boxB->GetPosition(), 2.0f, boxB->GetHeight(), outPosition
//			))
//			{
//				boxB->SetPosition(outPosition);
//			}
//		}
//	}
//}


//�{�b�N�X���m�̏Փˏ���
void BoxManager::CollisionBoxVsBox()
{
	
	size_t boxCount = boxes.size();
	for (int i = 0; i < boxCount; ++i)
	{
		Boxes* boxA = dynamic_cast<Boxes*>(boxes.at(i));
		for (int j = i + 1; j < boxCount; ++j)
		{
			Boxes* boxB = dynamic_cast<Boxes*>(boxes.at(j));

			DirectX::XMFLOAT3 outPosition;

			if (Collision::IntersecCylinderVsCylinder(
				boxA->GetPosition(), 2.0f, boxA->GetHeight(),
				boxB->GetPosition(), 2.0f, boxB->GetHeight(),
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
					boxB->SetPosition(outPosition);
				}
			}
		}
	}
}

//�v���C���[�ƃu���b�N�̓����蔻��
void BoxManager::CollisionBoxVsPlayer()
{
	size_t boxCount = boxes.size();
	for (int i = 0; i < boxCount; ++i)
	{
		Boxes* boxA = dynamic_cast<Boxes*>(boxes.at(i));
		if (boxA->GetColor() == BoxColor::PLAYER)
		{
			for (int j = i + 1; j < boxCount; ++j)
			{
				Boxes* boxB = dynamic_cast<Boxes*>(boxes.at(j));
				if (boxB->GetColor() != BoxColor::PLAYER)
				{
					DirectX::XMFLOAT3 outPosition;
					if (Collision::IntersecCylinderVsCylinder(
						boxA->GetPosition(), 2.0f, boxA->GetHeight(),
						boxB->GetPosition(), 2.0f, boxB->GetHeight(),
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



