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
		//ボックスを破棄
		delete box;
	}
	//破棄リストをクリア
	remove.clear();

	//ボックス同士の衝突処理
	CollisionBoxVsBox();

	//ボックスとプレイヤーの当たり判定
	CollisionBoxVsPlayer();
}

void BoxManager::Render(ID3D11DeviceContext* constext, Shader* shader)
{
	for (Box* box : boxes)
	{
		box->Render(constext, shader);
	}
}

//ボックス登録
void BoxManager::Register(Box* box)
{
	boxes.emplace_back(box);
}

//ボックス全削除
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
	//破棄リストに追加
	remove.insert(box);
}
//
//
////円柱と円柱
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


//ボックス同士の衝突処理
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
					//同じブロックの色同士が衝突したら、両方を削除
					Remove(boxA);
					Remove(boxB);
				}
				else
				{
					//異なる色のボックスが衝突した場合、AがBを押し出す
					boxB->SetPosition(outPosition);
				}
			}
		}
	}
}

//プレイヤーとブロックの当たり判定
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



