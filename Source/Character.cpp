#include "Character.h"
#include "Input/Input.h"
#include "Mathf.h"
#include "StageManager.h"
#include <Camera.h>
#include "CameraController.h"

//行列更新処理
void  Character::UpdateTransform()
{
	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x,scale.y,scale.z);

	//回転行列を作成
	//DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(this->angle.x,this->angle.y,this->angle.z);
	// X軸の回転行列
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	// Y軸の回転行列
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	// Z軸の回転行列
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);

	//回転行列を作成
	DirectX::XMMATRIX R = Y * X * Z;

	//位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x,position.y,position.z);//もし引数がtranceform._11,tranceform._22,tranceform._33だとその場で回転になる

	//3つの行列を組み合わせワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;

	//計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&tranceform, W);
}

bool Character::ApplyDamage(int damage, float invincbleTime)
{
	// ダメージが0の場合は健康状態を変更する必要がない
	if (damage <= 0) return false;

	// 死亡している場合は健康状態を変更しない
	if (health <= 0) return false;

	// ダメージ処理
	health -= damage;

	// 死亡通知
	if (health <= 0)
	{
		OnDead();
	}
	//ダメージ通知
	else
	{
		OrDamage();
		UpdateInvincTimer(invincbleTime);
	}
		

	//健康状態が変更した場合はtrueを返す
	return true; // ダメージが適用されたことを示す
}
//衝撃を与える
void Character::AddImpulse(const DirectX::XMFLOAT3& impilse)
{
	//速力に力を加える
	velocity.x += impilse.x;
	velocity.y += impilse.y;
	velocity.z += impilse.z;
}
//無敵時間更新
void Character::UpdateInvincTimer(float elapsedTime)
{
	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= elapsedTime;
	}
}


void Character::MoveVec(float vx, float vz, float speed)
{
	//方向ベクトルを設定
	moveVecX = vx;
	moveVecZ = vz;

	//最大速度処理
	maxMoveSpeed = speed;
}

void Character::Turn(float elaspedTime, float vx, float vz, float speed)
{
	speed *= elaspedTime;
	// 進行ベクトルが0ベクトルの場合処理する必要なし
	if (vx == 0.0f && vz == 0.0f)
	{
		return;
	}
	//進行ベクトルを単位ベクトル化
	float lenth = sqrtf(vx * vx + vz * vz);
	vx /= lenth;
	vz /= lenth;

	//自分の回転力毎方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	//自分の回転角を求めるため2つの単位ベクトルの内積を計算する
	float dot = vx * frontX + vz * frontZ;

	//内積値の2つの単位ベクトルの角度が小さいほど1.0に近づくという性質を利用して回転速度を調節する
	float rot = 1.0f - dot;
	if (speed > rot)speed = rot;

	//左右判定を行うために2つの単位ベクトルの外積を計算する
	float cross = (vz * frontX) - (vx * frontZ);

	//外積が正か負で左右を判定する
	if (cross < 0.0f)
	{
		//右の場合
		angle.y += speed;
	}
	else
	{
		//左の場合
		angle.y -= speed;
	}
}

void Character::Jump(float speed)
{
	velocity.y = speed;
}

void Character::UPdateVelocity(float elaspedTime)
{
	//経過フレーム
	float elaspedFrame = 60.0f * elaspedTime;

	//垂直速力更新処理
	UpdateVerticalVelocity(elaspedFrame);

	//垂直移動更新処理
	UpdateVerticalMove(elaspedTime);

	//水平速力更新処理
	UpdateHorizontalVelocity(elaspedFrame);

	//水平移動更新処理
	UpdateHorizontalMove(elaspedTime);

	//CharacterじゃなくてBoxesとPlayerのみに適応するようにする
	MoveWithCameraRotation(elaspedTime);

}

//垂直速力更新処理
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
//重力処理;
	velocity.y += gravity * elapsedFrame;
}


//垂直移動処理
void Character::UpdateVerticalMove(float elapsedTime)
{
	//垂直方向の移動量
	float my = velocity.y * elapsedTime;

	//キャラクターのY軸方向となる法線ベクトル
	DirectX::XMFLOAT3 normal = { 0,1,0 };
	
	//落下中
	if (my < 0.0f)
	{
		
		//レイの開始位置は足元より少し上
		DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z};
		//レイの終点位置は移動後の位置
		DirectX::XMFLOAT3 end = { position.x ,position.y + my,position.z };

		//レイキャストによる地面判定
		HitResult hit;
		
		if(StageManager::Instance().RayCast(start,end,hit))
		{
			//法線ベクトル取得
			normal = hit.normal;

			//地面に接触している
			position = hit.position;
			//回転
			angle.y += hit.rotation.y;
			
			//傾斜率の計算
			float normallengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
			slopeRate = 1.0f - (hit.normal.y / (normallengthXZ + hit.normal.y));

			//着地したら
			if (!isGround)
			{
				OnLanding();
			}
			isGround = true;
			velocity.y = 0.0f;
		}
		else
		{
			//空中に浮いていたら
			position.y += my;
			isGround = false;
		}
	}
	//上昇中
	else if (my> 0.0f)
	{
		position.y += my;
		isGround = false;
	}

#if false
	//地面の向きに沿うようにXZ軸回転
	{
		float Euler_angleX =  atan2f(normal.z, normal.y);
		float Euler_angleZ = -atan2f(normal.x, normal.y);

		//線形保管で滑らかに回転させる
		angle.x = Mathf::Lerp(angle.x, Euler_angleX, 0.2f);
		angle.z = Mathf::Lerp(angle.z, Euler_angleZ, 0.2f);

	}

	{
		//坂の方向に移動する速度を計算
		float slopeSpeed = slopeRate * elapsedTime * maxSlopeSpeed;
		//坂の方向に移動
		position.x += slopeSpeed * normal.x;
		position.z += slopeSpeed * normal.z;

		
	}
#endif
	
}

//水平速力更新処理
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	// XZ平面の速度を減衰する
	float length = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// 摩擦力
		float friiction = this->friiction * elapsedFrame;

		//空中に居る時は摩擦力を減らす
		if (!IsGround()) friiction *= (1.0f - airControl); // 摩擦力を30%減らす

		// 摩擦力による減衰処理
		if (length > friiction)
		{
			// 速度の方向を計算
			float directionX = velocity.x / length;
			float directionZ = velocity.z / length;

			// 新しい速度を計算
			velocity.x -= directionX * friiction;
			velocity.z -= directionZ * friiction;
		}
		else
		{
			// 速度が摩擦力以下になった場合は速度を0に設定
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}
	// XZ平面の速度を加速する
	if (length <= maxMoveSpeed)
	{
		// 移動ベクトルがゼロベクトルでない場合は加速する
		float moveVecLength = sqrt(moveVecX * moveVecX + moveVecZ * moveVecZ); // 移動ベクトルの長さを計算
		if (moveVecLength > 0.0f)
		{
			// 加速力
			float acceleration = this->acceleration * elapsedFrame;

			//下り坂でがたがたしないようにする
			if (isGround && slopeRate > 0.0f)
			{
				velocity.y -= length * slopeRate * elapsedFrame;
			}

			//空中にいる時は加速力を減らす
			if (!IsGround())acceleration *= (1.0f - airControl);//加速力を減らす

			// 移動ベクトルに加速処理
			velocity.x += (moveVecX * acceleration);
			velocity.z += (moveVecZ * acceleration);

			// 最大速度制限
			float newLength = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
			if (newLength > maxMoveSpeed)
			{
				// 速度を最大速度に制限
				float ratio = maxMoveSpeed / newLength;
				velocity.x *= ratio;
				velocity.z *= ratio;
			}
		}
	}

	// 移動ベクトルをリセット
	moveVecX = 0.0f;
	moveVecZ = 0.0f;

}

void Character::UpdateHorizontalMove(float elapsedTime)
{
	// 水平速力計算
	float velocityLengthXZ = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		// 水平移動値
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;

		// レイの開始位置と終点位置
		DirectX::XMFLOAT3 start = { position.x, position.y + 1.0f, position.z };
		DirectX::XMFLOAT3 end = { position.x + mx, position.y + 1.0f, position.z + mz };

		// レイキャストによる壁判定
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Ray = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&hit.position), End);

				//壁の法線
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			//入射ベクトルを法線に射影
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Ray, Normal);

			//補正位置の計算
			//法線ベクトルと進行方向のドット積を計算して、滑り幅(a)として使用
			float a = DirectX::XMVectorGetX(Dot);

			// 交点からの滑り先の位置を計算
			DirectX::XMVECTOR rVec = DirectX::XMVectorAdd(End, DirectX::XMVectorScale(Normal, a * 1.8f));
			DirectX::XMFLOAT3 R;
			DirectX::XMStoreFloat3(&R, rVec);

			HitResult hitwall;
			//hit.positionを開始位置として、Rを終点位置としてレイキャストをする
			if (!StageManager::Instance().RayCast(hit.position, R, hitwall))
			{
				
				//当たったら
				position.x = R.x;
				position.z = R.z;
			}
			else
			{
				////当たって居なかったらhitwall.positionを最終的な位置として繁栄
				position.x = hitwall.position.x;
				position.z = hitwall.position.z;
			}
		}
		else
		{
			// 壁に当たっていない場合、通常の移動
			position.x += mx;
			position.z += mz;
		}
	}
}

//// カメラの回転に基づいてキャラクターを移動させる 回転地が0の場合
//void Character::MoveWithCameraRotation(float elapsedTime)
//{
//
//	//カメラの回転角度を取得
//	CameraController& cameraController = CameraController::Instance();
//
//	DirectX::XMFLOAT3 cameraAngle = cameraController.GetAngle();
//
//	// カメラの回転に基づいてキャラクターの移動方向を計算
//	float moveDirectionX = cosf(DirectX::XM_PI / 2.0f - cameraAngle.x); // X軸に回転した場合、キャラクターはX方向に移動
//	float moveDirectionY = sinf(cameraAngle.y); // Y軸に回転した場合、キャラクターはY方向に移動
//	float moveDirectionZ = cosf(DirectX::XM_PI / 2.0f - cameraAngle.z); // Z軸に回転した場合、キャラクターはZ方向に移動
//
//
//	// 各軸に対応する移動量を計算
//	float moveAmountX = moveDirectionX * maxMoveSpeed * elapsedTime;
//	float moveAmountY = moveDirectionY * maxMoveSpeed * elapsedTime;
//	float moveAmountZ = moveDirectionZ * maxMoveSpeed * elapsedTime;
//
//	// キャラクターの位置を更新
//	position.x += moveAmountX;
//	position.y += moveAmountY;
//	position.z += moveAmountZ;
//}


// カメラの回転に基づいてキャラクターを移動させる 回転地がx90の場合
void Character::MoveWithCameraRotation(float elapsedTime)
{
	//カメラの回転角度を取得
	CameraController& cameraController = CameraController::Instance();

	DirectX::XMFLOAT3 cameraAngle = cameraController.GetAngle();

	// カメラの回転に基づいてキャラクターの移動方向を計算
	float moveDirectionX = sinf(cameraAngle.x); // X軸に回転した場合、キャラクターはX方向に移動
	float moveDirectionZ = sinf(cameraAngle.z); // Z軸に回転した場合、キャラクターはZ方向に移動

	// 各軸に対応する移動量を計算
	float moveAmountX = moveDirectionX * maxMoveSpeed * elapsedTime;
	float moveAmountZ = moveDirectionZ * maxMoveSpeed * elapsedTime;

	// キャラクターの位置を更新
	if (cameraAngle.x > DirectX::XM_PI / 2.0f && cameraAngle.x <= DirectX::XM_PI) {
		position.x -= moveAmountX; // z方向に移動
	}
	else if (cameraAngle.x < DirectX::XM_PI / 2.0f && cameraAngle.x >= 0) {
		position.x += moveAmountX; // -z方向に移動
	}
	// キャラクターの位置を更新
	if (cameraAngle.z > DirectX::XM_PI / 2.0f && cameraAngle.z <= DirectX::XM_PI) {
		position.z += moveAmountZ; // x方向に移動
	}
	else if (cameraAngle.z < DirectX::XM_PI / 2.0f && cameraAngle.z >= 0) {
		position.z -= moveAmountZ; // -x方向に移動
	}
}
