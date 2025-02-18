#pragma once

#include <DirectXMath.h>

//キャラクター
class Character
{
public:
	Character() {}
	virtual ~Character() {}

	//行列更新処理
	void UpdateTransform();

	//位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	//位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	//回転取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	//回転設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	//スケール取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	//スケール設定
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	//半径取得
	float GetRadius() const { return radius; }

	//健康状態を取得
	int GetHealth() const { return health; }

	//最大健康状態を取得
	int GetMaxHealth() const { return maxHealth; }

	//地面に接地しているか
	bool IsGround() const { return isGround; }

	//高さ取得
	float GetHeight() const { return height; }

	//ダメージを与える
	bool ApplyDamage(int damage, float incincibleTime);//無敵時間用の引数を追加

	//衝撃を与える
	void AddImpulse(const DirectX::XMFLOAT3& impilse); 
	
protected:

	//ダメージを受けた特に呼ばれる
	virtual void OrDamage() {};

	//死亡した時に呼ばれる
	virtual void OnDead() {}

	//着地したら呼ばれる
	virtual void OnLanding() {}//オーバーライド可能

	//無敵時間更新
	void UpdateInvincTimer(float elapsedTime);

	//移動処理
	// void Move(float elapsedTime, float vx, float vz, float speed);
	void MoveVec(float vx, float vz, float speed);

	//傾斜率で移動したベクトルを保持する関数
	//void SlopeMove(float vx, float vy, float vz, float slopSpeed);

	//旋回処理
	void Turn(float elaspedTime, float vx, float vz, float speed);

	//ジャンプ処理
	void Jump(float speed);

	//速力処理更新
	void UPdateVelocity(float elaspedTime);

private:
	//垂直速力更新処理
	void UpdateVerticalVelocity(float elapsedFrame);

	//垂直移動更新処理
	void UpdateVerticalMove(float elapsedTime);

	//水平速力更新処理
	void UpdateHorizontalVelocity(float elapsedFrame);

	//水平移動更新処理
	void UpdateHorizontalMove(float elapsedTime);

	//カメラの回転に基づいてキャラクターの移動を調整する関数
	void MoveWithCameraRotation(float elapsedTime);

	//Camera回転角度でCharacter水平移動更新処理
	//void UpdatePositionWithCameraRotation(float elapsedTime);

protected:
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 angle = { 0,0,0 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 tranceform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	float radius = 1.0f;

	float turnSpeed = DirectX::XMConvertToRadians(720);

	float jumpSpeed = 20.0f;

	float gravity = -50.0f;

	float moveSpeed = 5.0f;

	bool isGround = false;

	float height = 2.0f;

	int health = 5;

	int maxHealth = 5;

	float invincibleTimer = 1.0f;

	float friiction = 0.5f;

	float acceleration = 1.0f;

	float maxMoveSpeed = 1.5f;

	float moveVecX = 0.0f;

	float moveVecZ = 0.0f;

	float airControl = 0.3f;

	float stepOffset = 1.f;

	float slopeRate = 1.0f;

	float sllopeAngle = 0.0f;

	float maxSlopeSpeed = 10.0f;

	//進行ベクトルを計算


	DirectX::XMFLOAT3 velocity = { 0,0,0 };
};