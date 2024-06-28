#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//スライム
class  EnemySlime : public Enemy
{
public:
	 EnemySlime ();
	~EnemySlime() override;

	void Update(float elapsedTime) override;

	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//デバッグプリミティブ
	void DrawDebugPrimitive() override;

	//縄張り設定
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

private:
	//ノードとプレイヤーの衝突処理
	void CollisionNodeVsPlayer(const char* nodeName, float boneRadius);

	//ターゲット位置をランダムに設定
	void SetRandomTargetPosition();

	//プレイヤー索敵
	bool SartchPlayer();

	//目標地点へ移動
	void MoveToTarget(float elapsedTime, float speedRate);

	//徘徊ステートへ遷移
	void TransitionWanderState();

	//徘徊ステート更新処置
	void UpdateWanderState(float elapsedTime);

	//待機ステートに遷移
	void TransitionIdleState();

	//待機ステート更新処理
	void UpdateIdleState(float elapsedTime);

	//追跡ステートへ遷移
	void TransitionPursuitState();

	//追跡ステート更新処理
	void UpdatePursuitState(float elapsedTime);

	//攻撃ステートへ遷移
	void TransitionAttackState();

	//攻撃ステート更新処理
	void UpdateAttackState(float elapsedTime);

	//ダメージステートへ遷移
	void TransitionDamageState();

	//ダメージステート更新処理
	void UpdateDamageState(float elapsedTime);

	//死亡ステートへ遷移
	void TransitionDeathState();

	//死亡ステート更新処理
	void UpdateDeathState();

private:
	//ステート
	enum  class State
	{
		Wander,
		idle,
		Pursuit,
		Attack,
		Damage,
		Death
	};
	//アニメーション
	enum Animation
	{
		Anim_IdleNormal,
		Anim_IdleBattle,
		Anim_Attack1,
		Anim_Attack2,
		Anim_WalkFWD,
		Anim_WalkBWD,
		Anim_WalkLeft,
		Anim_WalkRight,
		Anim_RunFWD,
		Anim_SenseSomthingST,
		Anim_SenseSomthingPRT,
		Anim_Turn,
		Anim_Victory,
		Anim_GetHit,
		Anim_Dizzy,
		Anim_Die
	};

private:
	Model* model = nullptr;
	State state = State::Wander;

private:
	DirectX::XMFLOAT3 targetPosition = {};
	DirectX::XMFLOAT3 territoryOrigin ={};

private:
	float territoryRange = 10.0f;
	float moveSpeed = 3.0f;
	float turnSpeed = DirectX::XMConvertToRadians(360);
	float stateTimer = 0.0f;
	float searchRange = 5.0f;
	float attackRange = 1.5f;

protected:
	//死亡した時に呼ばれる
	void OnDead() override;

	//ダメージを受けたときに呼ばれる
	//void OnDamaged() override;
};

 