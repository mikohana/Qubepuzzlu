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
	position = DirectX::XMFLOAT3(5.0f, 0.0f, -3.0f);
	height = 1.0f;
	radius = 0.5f;

}
Goal::~Goal()
{
	delete hitEffect;

	delete model;
}


void Goal::Update(float elapedTime)
{

	//ボックスとゴールの判定
	//CollisionBoxesVsGoal();

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
//
////すべてのブロックとゴールの当たり判定
//void Goal::CollisionBoxesVsGoal()
//{
//	// BoxManagerからすべてのボックスを取得
//	BoxManager& boxManager = BoxManager::Instance();
//
//	std::vector<Boxes*> allBoxes = boxManager.GetAllBoxes();
//
//	// すべてのボックスとゴールの衝突判定
//	for (Boxes* box : allBoxes)
//	{
//		// ボックスがPlayerであるか確認
//		if (box->GetColor() == BoxColor::PLAYER)
//		{
//			DirectX::XMFLOAT3 outPosition;
//			if (Collision::IntersecCylinderVsCylinder(
//				box->GetPosition(), 2.0f, box->GetHeight(),
//				this->GetPosition(), 2.0f, this->GetHeight(),
//				outPosition
//			))
//			{
//				// Playerのボックスとゴールが衝突したらタイトルシーンに切り替え
//				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
//				break;  // 衝突が確認できたらループを抜ける
//			}
//		}
//	}
//}