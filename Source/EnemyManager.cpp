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
		//�G�l�~�[��j��
		delete enemy;
	}
	//�j�����X�g���N���A
	remove.clear();

	//�G���m�̏Փˏ���
	CollisionEnemyVsEnemies();
}

void EnemyManager::Render(ID3D11DeviceContext* constext, Shader* shader)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Render(constext, shader);
	}
}

//�G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
	enemies.emplace_back(enemy);
}

//�G�l�~�[�S�폜
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

void EnemyManager::Remove(Enemy* enemy)
{
	//�j�����X�g�ɒǉ�
	remove.insert(enemy);
}


//�~���Ɖ~��
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
			if (Collision::InstarsecCubeVsCube(
				enemyA->GetPosition(), 1.0f, enemyA->GetHeight(), enemyB->GetPosition(), 1.0f, enemyB->GetHeight(), outPosition
			))
			{
				enemyB->SetPosition(outPosition);
			}
		}
	}
}
