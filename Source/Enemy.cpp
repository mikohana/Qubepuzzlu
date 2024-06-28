#include "Enemy.h"
#include "EnemyManager.h"
#include "Graphics/Graphics.h"

void Enemy::Destroy()
{
	// EnemyManager�̃C���X�^���X���擾
	EnemyManager& enemyManager = EnemyManager::Instance();
	// EnemyManager���炱��Enemy���폜
	enemyManager.Remove(this);
}

void Enemy::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//�Փ˔���p�̃f�o�b�O����`��
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

	//�Փ˔���p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 1, 1, 0));
	
	
}
