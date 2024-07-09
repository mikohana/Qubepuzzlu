#include "BoxManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"



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


//円柱と円柱
void BoxManager::CollisionBoxVsBox()
{
	size_t boxCount = boxes.size();
	for (int i = 0; i < boxCount; ++i)
	{
		Box* boxA = boxes.at(i);
		for (int j = i + 1; j < boxCount; ++j)
		{
			Box* boxB = boxes.at(j);

			DirectX::XMFLOAT3 outPosition;
			
			if (Collision::InstarsecCubeVsCube(
				boxA->GetPosition(), 2.0f, boxA->GetHeight(), boxB->GetPosition(), 2.0f, boxB->GetHeight(), outPosition
			))
			{
				boxB->SetPosition(outPosition);
			}
		}
	}
}