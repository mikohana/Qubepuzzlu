#pragma once
#include <vector>
#include <set>
#include "Box.h"

//ボックスマネージャー
class  BoxManager : public Character
{
private:
	BoxManager() {};
	~BoxManager() {};

public:
	//インスタンスの取得
	static BoxManager& Instance()
	{
		static BoxManager instance;
		return instance;
	}


	void Update(float elapsedTime);

	void Render(ID3D11DeviceContext* dc, Shader* shader);
	//ボックス登録
	void Register(Box* box);

	void Clear();

	//デバッグプリミティブ描画
	void DrawDebugPrimitive();

	//ボックス数取得
	int GetBoxCount() const { return static_cast<int>(boxes.size()); }

	//ボックス取得
	Box* GetBox(int index) const { return boxes.at(index); }

	//ボックス削除
	void Remove(Box* box);


private:
	//ボックス同士の衝突処理
	void CollisionBoxVsBox();

	std::set<Box*> remove;

	std::vector<Box*> boxes;


};

