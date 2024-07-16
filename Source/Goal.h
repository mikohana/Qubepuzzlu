#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectilrManager.h"
#include "Effect.h"

//ゴール
class Goal : public Character
{
public:
	Goal();
	~Goal() override;

	//インスタンス取得
	static Goal& Instance();

	//更新処理
	void Update(float elapedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	////すべてのボックスとゴールの当たり判定
	//void CollisionBoxesVsGoal();

private:
	Model* model = nullptr;

	ProjectileManager projectileManager;

	Effect* hitEffect = nullptr;


};