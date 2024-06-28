#pragma once
#include <vector>
#include<set>
#include "Projectile.h"

//�e�ۃ}�l�[�W���[
class ProjectileManager : public Projectile
{
public:

	 ProjectileManager();
	 ~ProjectileManager();

	 //�X�V����
	 void Update(float elaspedTime);

	 //�`�揈��
	 void Render(ID3D11DeviceContext* dc, Shader* shader);

	 //�f�o�b�O�v���~�e�B�u�`��
	 void DebugPrimitive();

	 //�e�ۓo�^
	 void Register(Projectile* projectile);

	 //�e�ۍ폜
	 void Remove(Projectile* projectile);
	 //�e�ۑS����
	 void Clear();

	 //�e�ې��擾
	 int GetProjectileCount() const { return static_cast<int>(projectiles.size()); }

	 //�e�ێ擾
	 Projectile* GetProjectile(int index) { return projectiles.at(index); }

private:
	
	std::vector<Projectile*> projectiles;

	std::set<Projectile*> removes;
};