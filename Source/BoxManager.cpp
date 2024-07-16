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
		//ボックスを破棄
		delete box;
	}
	//破棄リストをクリア
	remove.clear();

	//ボックス同士の衝突処理
	CollisionBoxVsBox();

	//ボックスとプレイヤーの当たり判定
	CollisionBoxVsPlayer();

	//Playerとゴールの当たり判定
	CollisionBoxesVsGoal();

}

void BoxManager::Render(ID3D11DeviceContext* constext, Shader* shader)
{
	for (Box* box : box)
	{
		box->Render(constext, shader);
	}
}

//ボックス登録
void BoxManager::Register(Box* box)
{
	this->box.emplace_back(box);
}

//ボックス全削除
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
	//破棄リストに追加
	remove.insert(box);
}


//ボックス同士の衝突処理
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
					//同じブロックの色同士が衝突したら、両方を削除
					Remove(boxA);
					Remove(boxB);
				}
				else
				{
					//異なる色のボックスが衝突した場合、AがBを押し出す
					if(boxB->GetColor() != BoxColor::GOAL)
					boxB->SetPosition(outPosition);
				}
			}
		}
	}
}

//プレイヤーとブロックの当たり判定
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

//プレイヤーとゴールの当たり判定
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
						// PlayerのボックスとGoalが衝突したらタイトルシーンに切り替え
						SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
						return;
					}
				}
			}
		}
	}
}
