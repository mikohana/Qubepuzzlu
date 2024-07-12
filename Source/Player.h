#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectilrManager.h"
#include "Effect.h"

//プレイヤー
class Player : public Character
{
public:
	Player();
	~Player() override;

	//インスタンス取得
	static Player& Instance();

	//更新処理
	void Update(float elapedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//デバッグ用GUI描画処理
	void DrawDebugGuI();

	//当たり判定
	void DrawDebugPrimitive();

	//プレイヤーとエネミーとの衝突判定
	void CollisionPlayerEnemies();

	

	//弾丸と敵の衝突処理
	void CollisionProjectileVsEnemies();

	//ノードとエネミーの衝突判定
	void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

	//弾丸入力処理
	void InputProjectile();

	
protected:

	//着地処理
	void OnLanding() override;

private:
	//アニメーション
	enum  Animation
	{
		Anim_Attack,
		Anim_Death,
		Anim_Falling,
		Anim_GetHit1,
		Anim_GetHit2,
		Anim_Idie,
		Anim_Jump,
		Anim_Jump_Flip,
		Anim_Landing,
		Anim_Revive,
		Anim_Running,
		Anim_Walking
	};
private:
	Model* model = nullptr;

	ProjectileManager projectileManager;

	Effect* hitEffect = nullptr;

	//インスタンス取得
private:
	//スティックの入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;
	//移動入力処理
	bool  InputMove(float elapsedTime);

	//ジャンプ入力処理
	bool InputJump();

	//攻撃入力処理
	bool InputArrack();

	//待機ステートへ遷移
	void TransitionIdleState();

	//待機ステート更新処理
	void UpdateIdleState(float elapsedTime);

	//移動ステートへ遷移
	void TransitionMoveState();

	//移動ステート更新処理
	void UpdateMoveState(float elapsedTime);

	//ジャンプステートに遷移
	void TranstionJumpState();

	//ジャンプステート更新処理
	void UpdateJumpState(float elapsedTime);

	//着地ステートに遷移
	void TransitionLamdState();

	//着地ステート更新処理
	void UpdateLandState(float elapsedTime);

	//攻撃ステートへ遷移
	void TransitionAttackState();

	//攻撃ステート更新処理
	void UpdateAttackState(float elapsedTime);
	
private:
	//ステート
	enum class State
	{
		Idle,
		Move,
		Jump,
		Land,
		Attack
	};
	State state = State::Idle;

	int jumpCount = 0;
	int jumpLimit = 2;
	float leftHandRadius = 0.4f;
	bool attackCollisionFlag = false;
};