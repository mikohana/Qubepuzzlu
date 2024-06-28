#include<imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "PrjectileHoming.h"
//using namespace ImGui;

//インスタンス変数
static Player* instance = nullptr;

//インスタンス取得
Player& Player::Instance()
{
	return *instance;
}

Player::Player()
{
	//インスタンスポインタ設定
	instance = this;

	//model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
	model = new Model("Data/Model/Jammo/Jammo.mdl");

	//ヒットエフェクト読み込み
	hitEffect = new Effect("Data/Effect/Hit.efk");

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

	//待機ステートへ遷移
	TransitionIdleState();
}
Player::~Player()
{
	delete hitEffect;

	delete model;
}

void Player::Update(float elapedTime)
{
	////Bボタンでワンショットアニメーション再生
	//GamePad& gamePad = Input::Instance().GetGamePad();
	//if (gamePad.GetButtonDown() & GamePad::BTN_B)
	//{
	//	model->PlayAnimation(Anim_Running, true);
	//}
	////ワンショットアニメーション再生が終わったらループアニメーションを再生
	//if (!model->IsPlayAnimation())
	//{
	//	model->PlayAnimation(Anim_Running, true);
	//}

	////移動入力処理
	//InputMove(elapedTime);
	//
	////ジャンプ入力処理
	//InputJump();

	//プレイヤーとエネミーの衝突判定
	CollisionPlayerEnemies();

	CollisionProjectileVsEnemies();

	// 速力処理更新
	UPdateVelocity(elapedTime);

	//弾丸更新処理
	projectileManager.Update(elapedTime);

	////弾丸入力処理
	//InputProjectile();

	//オブジェクト行列を更新
	UpdateTransform();

	//モデルアニメーション更新処理
	model->UpdateAnimation(elapedTime);
	
	//モデル行列更新
	model->UpdateTransform(tranceform);

	//ステート毎の更新処理
	switch (state)
	{
	case State::Idle:
		UpdateIdleState(elapedTime);
		break;
	case State::Move:
		UpdateMoveState(elapedTime);
		break;
	case State::Jump:
		UpdateJumpState(elapedTime);
		break;
	case State::Land:
		UpdateLandState(elapedTime);
		break;
	case State::Attack:
		UpdateAttackState(elapedTime);
		break;
	}
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	//プレイヤー描画
	shader->Draw(dc, model);
	
	//弾丸描画
	projectileManager.Render(dc, shader);
}

void Player::DrawDebugGuI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if(ImGui::Begin("Player",nullptr,ImGuiWindowFlags_None))
	{
		//トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//位置
			ImGui::InputFloat3("Postion", &position.x);

			//回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			
			//スケール
			ImGui::InputFloat3("Scale", &scale.x);
			ImGui::Text("slopeRate : %f", slopeRate);
		}

		if (ImGui::CollapsingHeader("MoveMent", ImGuiTreeNodeFlags_DefaultOpen))
		{
			
		}

		if (ImGui::CollapsingHeader("Collison", ImGuiTreeNodeFlags_DefaultOpen))
		{

		}
	}
	ImGui::End();
}

//移動処理
//void Player::Move(float elapsedTime, float vx, float vz, float speed)
//{
//	speed *= elapsedTime;
//	position.x += vx * speed;
//	position.z += vz * speed;
//}

//移動入力処理
bool Player::InputMove(float elapsedTime)
{
	//進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	Move(moveVec.x, moveVec.z, moveSpeed);

	//旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

	//進行ベクトルがゼロベクトルではない場合に入力された
	return (moveVec.x != 0.0f || moveVec.z != 0.0f);


}

//デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//衝突判定用のデバッグ球を描画
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1)); //TODO06で円柱に変更

	//衝突判定用のデバック円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

	//弾丸デバッグプリミティブ描画
	projectileManager.DebugPrimitive();

	//攻撃衝突用の左手ノードのデバッグ球を描画
	if (attackCollisionFlag)
	{
		Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
		debugRenderer->DrawSphere(DirectX::XMFLOAT3(
			leftHandBone->worldTransform._41,
			leftHandBone->worldTransform._42,
			leftHandBone->worldTransform._43),
			leftHandRadius,
			DirectX::XMFLOAT4(1, 0, 0, 1)
		);
	}
	
}

//プレイヤーとエネミー衝突判定
void Player::CollisionPlayerEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//プレイヤーvsエネミー衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//衝突判定
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersecCylinderVsCylinder(
			Player::position,Player::radius,Player::height,
			enemy->GetPosition(),enemy->GetRadius(),enemy->GetHeight(),
			outPosition
		))
		{
			//敵の真上に当たったか
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, N);
			//上から踏んづけた場合は小ジャンする
			if (normal.y > 0.8f)
			{
				//小ジャンする
				Jump(jumpSpeed * 0.5f);
			}
			else
			{
				//押し出し後の位置設定
				enemy->SetPosition(outPosition);
			}
		}
	}
}


//弾丸と敵の衝突処理
void Player::CollisionProjectileVsEnemies()
{

	EnemyManager& enemyManager = EnemyManager::Instance();

	//全ての弾丸と全ての敵を総当たりで衝突処理
	int projectileCount = projectileManager.GetProjectileCount();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Projectile* projectile = projectileManager.GetProjectile(i);
		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* enemy = enemyManager.GetEnemy(j);

			//衝突処理
			DirectX::XMFLOAT3 outPosition;
			if (Collision::InstarsecSphereVsCylinder(
				projectile->GetPosition(), projectile->GetRadius(),
				enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(),
				outPosition
			))
			{
				// ダメージを与える
				if (enemy->ApplyDamage(1, 0.5f))
				{
					// 弾丸破棄
					projectile->Destroy();
					// 吹き飛ばす
					{
						DirectX::XMFLOAT3 direction;
						DirectX::XMFLOAT3 impulse;

						// 吹き飛ばす方向を計算
						DirectX::XMFLOAT3 position1 = enemy->GetPosition();
						DirectX::XMFLOAT3 position2 = outPosition;

						DirectX::XMVECTOR vector1 = DirectX::XMLoadFloat3(&position1);
						DirectX::XMVECTOR vector2 = DirectX::XMLoadFloat3(&position2);

						// 2つのベクトルの差を計算
						DirectX::XMVECTOR differenceVector = DirectX::XMVectorSubtract(vector1, vector2);

						// 結果をXMFLOAT3型に変換して格納
						DirectX::XMStoreFloat3(&direction, differenceVector);

						// 吹き飛ばす方向を正規化
						DirectX::XMVECTOR normalizedDirectionVector = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction));


						float force = 10.0f;
						DirectX::XMVECTOR forceVector = DirectX::XMVectorScale(normalizedDirectionVector, force);

						DirectX::XMStoreFloat3(&impulse, forceVector);

						enemy->AddImpulse(impulse);
						DirectX::XMFLOAT3 e = enemy->GetPosition();
						e.y += enemy->GetHeight() * 0.5f;
						hitEffect->Play(e);
					}
					//ヒットエフェクト再生
					{
						DirectX::XMFLOAT3 e = enemy->GetPosition();
						e.y += enemy->GetHeight() * 0.5f;
						hitEffect->Play(e);
					}
				};
			}
		}
	}
}

//ノードと敵の衝突処理
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
	//ノード取得
	Model::Node* node = model->FindNode(nodeName);

	//ノード位置取得
	DirectX::XMFLOAT3 nodePosition;
	if (node != nullptr)
	{
		nodePosition.x = node->worldTransform._41;
		nodePosition.y = node->worldTransform._42;
		nodePosition.z = node->worldTransform._43;
	}
	else
	{
		return;
	}
	
	//指定のノードと全ての敵を総当たりで衝突処理
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyConut = enemyManager.GetEnemyCount();
	for (int i=0; i < enemyConut; i++)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		//衝突判定
		DirectX::XMFLOAT3 outPosition;
		if (Collision::InstarsecSphereVsCylinder(
			nodePosition, nodeRadius,
			enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(),
			outPosition
		))
		{ 
		//ダメージを与える
			if (enemy->ApplyDamage(1, 0.5f))
			{
				//吹き飛ばす
				{
					DirectX::XMFLOAT3 direction;
					DirectX::XMFLOAT3 impulse;

					//吹き飛ばす方向を計算
					DirectX::XMFLOAT3 enemyPosition = enemy->GetPosition();
					DirectX::XMFLOAT3 outPos = outPosition;

					DirectX::XMVECTOR enemyVec = DirectX::XMLoadFloat3(&enemyPosition);
					DirectX::XMVECTOR outVec = DirectX::XMLoadFloat3(&outPos);

					//2つのベクトルの差を計算
					DirectX::XMVECTOR differenceVector = DirectX::XMVectorSubtract(enemyVec, outVec);

					//結果をZMFLOAT型に変換して格納
					DirectX::XMStoreFloat3(&direction, differenceVector);

					//吹き飛ばす方向を正規化
					DirectX::XMVECTOR norma = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction));

					float force = 10.0f;
					DirectX::XMVECTOR forceVec = DirectX::XMVectorScale(norma, force);

					DirectX::XMStoreFloat3(&impulse, forceVec);

					enemy->AddImpulse(impulse);
				}
				//ヒットエフェクト再生
				{
					DirectX::XMFLOAT3 e = enemy->GetPosition();
					e.y += enemy->GetHeight() * 0.5f;
					hitEffect->Play(e);

				}
			};
		}
	}
}

void Player::InputProjectile()
{
	GamePad& gamepad = Input::Instance().GetGamePad();

	//直線弾丸発射
	if (gamepad.GetButtonDown() & GamePad::BTN_X)
	{
		// プレイヤーの向きを取得
		DirectX::XMFLOAT3 playerForward;
		DirectX::XMVECTOR forwardVector = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMQuaternionRotationRollPitchYaw(Player::GetAngle().x, Player::GetAngle().y, Player::GetAngle().z));
		DirectX::XMStoreFloat3(&playerForward, forwardVector);
		// 発射方向を計算
		DirectX::XMFLOAT3 dir = playerForward; // プレイヤーの向きが発射方向


		// 発射位置をプレイヤーの位置に設定
		DirectX::XMFLOAT3 pos;//プレイヤーの位置が発射位置
		pos.x = GetPosition().x;
		pos.y = GetPosition().y + 0.5f;
		pos.z = GetPosition().z;

		//発射
		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
		//projectileManager.Register(projectile); 弾丸クラスのコンストラクタで呼び出しをするので不要
	}
	
	if (gamepad.GetButtonDown() & GamePad::BTN_Y)
	{
		// プレイヤーの向きを取得
		DirectX::XMFLOAT3 playerForward;
		DirectX::XMVECTOR forwardVector = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMQuaternionRotationRollPitchYaw(Player::GetAngle().x, Player::GetAngle().y, Player::GetAngle().z));
		DirectX::XMStoreFloat3(&playerForward, forwardVector);
		// 発射方向を計算
		DirectX::XMFLOAT3 dir = playerForward; // プレイヤーの向きが発射方向

		// 発射位置をプレイヤーの位置に設定
		DirectX::XMFLOAT3 pos;//プレイヤーの位置が発射位置
		pos.x = GetPosition().x;
		pos.y = GetPosition().y + 0.5f;
		pos.z = GetPosition().z;

		DirectX::XMFLOAT3 target;
		target.x = position.x + dir.x * 100.0f;
		target.y = position.y + dir.y * 100.0f;
		target.z = position.z + dir.z * 100.0f;

		//一番近くのエネミーをターゲティング
		float dist = FLT_MAX;
		EnemyManager& enemyManager = EnemyManager::Instance();
		int enemyCount = enemyManager.GetEnemyCount();
		for (int i = 0; i < enemyCount; i++)
		{
			//敵との距離判定
			Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);            //プレイヤーのposition
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());//エネミーのposition
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);             //エネミーとプレイヤーのpositionをベクトル化
			DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
			float d;//direction
			DirectX::XMStoreFloat(&d, D);

			if (d < dist)
			{
				dist = d;
				target = enemy->GetPosition();
				target.y += enemy->GetHeight() * 0.5f;
			}

			//発射
			ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
			projectile->Launch(dir, pos, target);

		}
	}
}


DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //   入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //   カメラ方向とスティックの入力値によって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //   移動ベクトルはXZ平面に水平なベクトルになるようにする
    //   カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength >  0.0f)
    {
        //  単位ベクトル化
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    //   カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z; //  Z成分を取得
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength >  0.0f)
    {
        //  単位ベクトル化
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //   スティックの水平入力値xをカメラ右方向に反映し、
    //   スティックの垂直入力値zをカメラ前方向に反映し、
    //   進行ベクトルを計算する
    DirectX::XMFLOAT3 vec3;
	vec3.x = cameraRightX * ax + cameraFrontX * ay ;
    vec3.z = cameraRightZ * ax + cameraFrontZ * ay;

    //  y軸方向には移動しない
    vec3.y =  0.0f;

    return vec3;
}

bool Player::InputJump()
{
	//ジャンプ制限あり
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		if (jumpCount < jumpLimit)
		{
			Jump(jumpSpeed);
			jumpCount++;
			//ジャンプ入力をした
			return true;
		}
	}
	return false;
}

bool Player::InputArrack()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_B)
	{
		return true;
	}
	return false;
}

//着地した時に呼ばれる
void Player::OnLanding()
{
	//着地した時にジャンプ回数をリセット
	jumpCount = 0;

	//下方向の速力が一定以上なら着地ステートに
	if (velocity.y != 0.0f)
	{
		TransitionLamdState();
	}
}

//待機ステートへ遷移
void Player::TransitionIdleState()
{
	state = State::Idle;

	//待機アニメーション再生
	model->PlayAnimation(Anim_Idie, true);
}

//待機ステート更新処理
void Player::UpdateIdleState(float elapsedTime)
{
	//移動入力処理
	if (InputMove(elapsedTime))
	{
		TransitionMoveState();
	}
	//ジャンプ入力処理
	if (InputJump())
	{
		TranstionJumpState();
	}

	//攻撃入力処理
	if (InputArrack())
	{
		TransitionAttackState();
	}

	//弾丸入力処理
	InputProjectile();
}

//移動ステートへ遷移
void Player::TransitionMoveState()
{
	state = State::Move;

	//走りモーション再生
	model->PlayAnimation(Anim_Running, true);
}

//移動ステート更新処理
void Player::UpdateMoveState(float elapsedTime)
{
	//移動未入力処理
	if (!InputMove(elapsedTime))
	{
		TransitionIdleState();
	}

	//ジャンプ入力処理
	if (InputJump())
	{
		TranstionJumpState();
	}

	//攻撃入力処理
	if (InputArrack())
	{
		TransitionAttackState();
	}

	//弾丸入力処理
	InputProjectile();
}

//ジャンプステートに遷移
void Player::TranstionJumpState()
{
	state = State::Jump;

	//ジャンプアニメーション再生
	model->PlayAnimation(Anim_Jump, false);
}

//ジャンプステート更新処理
void Player::UpdateJumpState(float elapsedTime)
{
	//移動入力処理
	if (!InputMove(elapsedTime))
	{
		return;
	}

	//ジャンプ入力処理
	if (InputJump())
	{
		TranstionJumpState();
	}
	
}

//着地ステートに遷移
void Player::TransitionLamdState()
{
	state = State::Land;

	//着地アニメーション再生
	model->PlayAnimation(Anim_Landing, false);
}

//着地ステート更新処理
void Player::UpdateLandState(float elapsedTime)
{
	//着地中,移動入力が合う場合に他の状態への遷移を行う
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}
	//弾丸入力処理
	InputProjectile();
}

//攻撃ステートに遷移
void Player::TransitionAttackState()
{
	state = State::Attack;

	//攻撃アニメーション再生
	model->PlayAnimation(Anim_Attack, false);
}

//攻撃ステート更新処理
void Player::UpdateAttackState(float elapsedTime)
{
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}

	//攻撃ボタンが押されたら
	if (InputArrack() && !model->IsPlayAnimation())
	{
		TransitionAttackState();
	}

	//任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentAnimationSeconds();
	attackCollisionFlag = (animationTime >= 0.3f && animationTime <= 0.4f);
	
	if (attackCollisionFlag)
	{
		//左手ノードとエネミーの衝突処理
		CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
		
	}


}
