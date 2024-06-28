#include "ProjectilrManager.h"



ProjectileManager::ProjectileManager() : Projectile(this)//this�Ŏ������g���|�C���^��n��
{
}

ProjectileManager::~ProjectileManager()
{
	Clear();
}

void ProjectileManager::Update(float elaspedTime)
{
	for (Projectile* projectile : projectiles)
	{
		projectile->Update(elaspedTime);
	}

	//�j������
	for (Projectile* projectile : removes)
	{
		//vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);
		if (it != projectiles.end())
		{
			projectiles.erase(it);
		}
		//�e�ۂ�j��
		delete projectile;
	}
	//�j�����X�g���N���A
	removes.clear();
}

void ProjectileManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (Projectile* projectile : projectiles)
	{
		projectile->Render(dc, shader);
	}
}

void ProjectileManager::DebugPrimitive()
{
	DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();
	for (Projectile* projectile : projectiles)
	{
		debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4{ 0,0,0,1 });
	}
}

void ProjectileManager::Register(Projectile* projectile)
{
	if (projectile != nullptr)
	{
		projectiles.push_back(projectile);
	}
}

void ProjectileManager::Remove(Projectile* projectile)
{
	//�j�����X�g�ɒǉ�
	removes.insert(projectile);
}

void ProjectileManager::Clear()
{
	for (Projectile* projectile : projectiles)
	{
		delete projectile;
	}
	projectiles.clear();
}


