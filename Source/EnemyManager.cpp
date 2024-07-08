#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"



void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}
	
	for (Enemy* enemy : remove)
	{
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
		if (it != enemies.end())
		{
			enemies.erase(it);
		}
		//エネミーを破棄
		delete enemy;
	}
	//破棄リストをクリア
	remove.clear();

	//敵同士の衝突処理
	CollisionEnemyVsEnemies();
}

void EnemyManager::Render(ID3D11DeviceContext* constext, Shader* shader)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Render(constext, shader);
	}
}

//エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
	enemies.emplace_back(enemy);
}

//エネミー全削除
void EnemyManager::Clear()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

void EnemyManager::DrawDebugPrimitive()
{
	DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();

	debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4{ 0,0,0,1 });
}

//球と球
//void EnemyManager::CollisionEnemyVsEnemies()
//{
//	EnemyManager& enemyManager = EnemyManager::Instance();
//
//	int enemyCount = enemyManager.GetEnemyCount();
//	for (int i = 0; i < enemyCount; i++)
//	{
//		Enemy* enemy1 = GetEnemy(i);
//
//		for (int j = i + 1; j < enemyCount; j++)
//		{
//			Enemy* enemy2 = GetEnemy(j);
//
//			DirectX::XMFLOAT3 outPosition;
//
//			// 衝突検出
//			if (Collision::IntrresectVsSphere(
//				enemy1->GetPosition(), enemy1->GetRadius(),
//				enemy2->GetPosition(), enemy2->GetRadius(),
//				outPosition
//			))
//			{
//				//押し出し後の位置
//				enemy2->SetPosition(outPosition);
//			}
//		}
//	}
//}

void EnemyManager::Remove(Enemy* enemy)
{
	//破棄リストに追加
	remove.insert(enemy);
}


//円柱と円柱
void EnemyManager::CollisionEnemyVsEnemies()
{
	size_t enemyCount = enemies.size();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemyA = enemies.at(i);
		for (int j = i + 1; j < enemyCount; ++j)
		{
			Enemy* enemyB = enemies.at(j);

			DirectX::XMFLOAT3 outPosition;
			/*if (Collision::IntersecCylinderVsCylinder(
				enemyA->GetPosition(),
				enemyA->GetRadius(),
				enemyA->GetHeight(),
				enemyB->GetPosition(),
				enemyB->GetRadius(),
				enemyB->GetHeight(),
				outPosition))
			{
				enemyB->SetPosition(outPosition);
			}*/
			if (Collision::InstarsecCubeVsCube(
				enemyA->GetPosition(), 1.0f, enemyA->GetHeight(), enemyB->GetPosition(), 1.0f, enemyB->GetHeight(), outPosition
			))
			{
				enemyB->SetPosition(outPosition);
			}
		}
	}
}
