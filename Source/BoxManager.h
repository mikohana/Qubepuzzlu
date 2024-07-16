#pragma once
#include <vector>
#include <set>
#include "Box.h"
#include "Boxes.h"
#include "Goal.h"
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
	int GetBoxCount() const { return static_cast<int>(box.size()); }

	//ボックス取得
	Box* GetBox(int index) const { return box.at(index); }

	//特定の種類のボックスを取得するメソッド
	Boxes* GetBoxByColor(BoxColor color)
	{
		//登録されている全てのボックスを定義
		for (Boxes* box : boxes)
		{
			//ボックスの色の指定された色と一致する場合、そのボックスを返す
			if (box->GetColor() == color)
			{
				return box;
			}
		}
		//指定された色のボックスがなかったらnullptr
		return nullptr;
	}

	//すべてのボックスを取得するメソッド
	std::vector<Boxes*> GetAllBoxes() const
	{
		return boxes;
	}
	//ボックス削除
	void Remove(Box* box);



private:
	//ボックス同士の衝突処理
	void CollisionBoxVsBox();

	//ボックスとプレイヤーの当たり判定
	void CollisionBoxVsPlayer();

	//すべてのボックスとゴールの当たり判定
	void CollisionBoxesVsGoal();

	std::set<Box*> remove;

	std::vector<Box*> box;

	std::vector<Boxes*> boxes; // ボックスのリスト
};

