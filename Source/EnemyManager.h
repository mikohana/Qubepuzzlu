#pragma once

#include <vector>
#include <set>
#include "Enemy.h"

//�G�l�~�[�}�l�[�W���[
class  EnemyManager : public Character
{
private:
	EnemyManager() {};
	~EnemyManager() {};

public:
	//�C���X�^���X�̎擾
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}


	void Update(float elapsedTime);

	void Render(ID3D11DeviceContext* dc, Shader* shader);
	//�G�l�~�[�o�^
	void Register(Enemy* enemy);

	void Clear();

	//�f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	//�G�l�~�[���擾
	int GetEnemyCount() const { return static_cast<int>(enemies.size());}

	//�G�l�~�[�擾
	Enemy* GetEnemy(int index) const { return enemies.at(index); }

	//�G�l�~�[�폜
	void Remove(Enemy* enemy);

	
private:
	// �G�l�~�[���m�̏Փˏ���
	void CollisionEnemyVsEnemies();

	std::set<Enemy*> remove;

	std::vector<Enemy*> enemies;


};

