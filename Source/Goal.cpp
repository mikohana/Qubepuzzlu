#include<imgui.h>
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Boxes.h"
#include "BoxManager.h"
#include "Goal.h"
#include "Collision.h"

#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"

//インスタンス変数
static Goal* instance = nullptr;

//インスタンス取得
Goal& Goal::Instance()
{
	return *instance;
}


Goal::Goal()
{
	//インスタンスポインタ設定
	instance = this;

	//model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
	model = new Model("Data/Model/Star/Star.mdl");

	//ヒットエフェクト読み込み
	hitEffect = new Effect("Data/Effect/Hit.efk");

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

}
Goal::~Goal()
{
	delete hitEffect;

	delete model;
}


void Goal::Update(float elapedTime)
{
	
	//プレイヤーとエネミーの衝突判定
	void CollisionPlayerVsGoal();

	UpdateTransform();

	//モデルアニメーション更新処理
	model->UpdateAnimation(elapedTime);

	//モデル行列更新
	model->UpdateTransform(tranceform);

}


void Goal::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	//ゴール描画
	shader->Draw(dc, model);
}


//プレイヤーとゴールの当たり判定
void Goal::CollisionPlayerVsGoal()
{
	// プレイヤーのインスタンスを取得
	Player& player = Player::Instance();

	// プレイヤーとゴールの衝突判定
	DirectX::XMFLOAT3 outPosition;
	if (Collision::IntersecCylinderVsCylinder(
		player.GetPosition(), 2.0f, player.GetHeight(),
		this->GetPosition(), 2.0f, this->GetHeight(),
		outPosition
	))
	{
		// 当たったら
		//ゴールの位置を更新する
		this->SetPosition(outPosition);
	}
}