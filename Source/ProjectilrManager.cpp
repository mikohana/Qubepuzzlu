#include "ProjectilrManager.h"



ProjectileManager::ProjectileManager() : Projectile(this)//thisで自分自身をポインタを渡す
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

	//破棄処理
	for (Projectile* projectile : removes)
	{
		//vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);
		if (it != projectiles.end())
		{
			projectiles.erase(it);
		}
		//弾丸を破棄
		delete projectile;
	}
	//破棄リストをクリア
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
	//破棄リストに追加
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


