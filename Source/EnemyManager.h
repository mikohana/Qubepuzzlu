#pragma once

#include <vector>
#include <set>
#include "Enemy.h"

//エネミーマネージャー
class  EnemyManager : public Character
{
private:
	EnemyManager() {};
	~EnemyManager() {};

public:
	//インスタンスの取得
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}


	void Update(float elapsedTime);

	void Render(ID3D11DeviceContext* dc, Shader* shader);
	//エネミー登録
	void Register(Enemy* enemy);

	void Clear();

	//デバッグプリミティブ描画
	void DrawDebugPrimitive();

	//エネミー数取得
	int GetEnemyCount() const { return static_cast<int>(enemies.size());}

	//エネミー取得
	Enemy* GetEnemy(int index) const { return enemies.at(index); }

	//エネミー削除
	void Remove(Enemy* enemy);

	
private:
	// エネミー同士の衝突処理
	void CollisionEnemyVsEnemies();

	std::set<Enemy*> remove;

	std::vector<Enemy*> enemies;


};

