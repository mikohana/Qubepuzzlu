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

//�C���X�^���X�ϐ�
static Player* instance = nullptr;

//�C���X�^���X�擾
Player& Player::Instance()
{
	return *instance;
}

Player::Player()
{
	//�C���X�^���X�|�C���^�ݒ�
	instance = this;

	//model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
	model = new Model("Data/Model/Jammo/Jammo.mdl");

	//�q�b�g�G�t�F�N�g�ǂݍ���
	hitEffect = new Effect("Data/Effect/Hit.efk");

	//���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

	//�ҋ@�X�e�[�g�֑J��
	TransitionIdleState();
}
Player::~Player()
{
	delete hitEffect;

	delete model;
}

void Player::Update(float elapedTime)
{
	////B�{�^���Ń����V���b�g�A�j���[�V�����Đ�
	//GamePad& gamePad = Input::Instance().GetGamePad();
	//if (gamePad.GetButtonDown() & GamePad::BTN_B)
	//{
	//	model->PlayAnimation(Anim_Running, true);
	//}
	////�����V���b�g�A�j���[�V�����Đ����I������烋�[�v�A�j���[�V�������Đ�
	//if (!model->IsPlayAnimation())
	//{
	//	model->PlayAnimation(Anim_Running, true);
	//}

	////�ړ����͏���
	//InputMove(elapedTime);
	//
	////�W�����v���͏���
	//InputJump();

	//�v���C���[�ƃG�l�~�[�̏Փ˔���
	CollisionPlayerEnemies();

	CollisionProjectileVsEnemies();

	// ���͏����X�V
	UPdateVelocity(elapedTime);

	//�e�ۍX�V����
	projectileManager.Update(elapedTime);

	////�e�ۓ��͏���
	//InputProjectile();

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	//���f���A�j���[�V�����X�V����
	model->UpdateAnimation(elapedTime);
	
	//���f���s��X�V
	model->UpdateTransform(tranceform);

	//�X�e�[�g���̍X�V����
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
	//�v���C���[�`��
	shader->Draw(dc, model);
	
	//�e�ە`��
	projectileManager.Render(dc, shader);
}

void Player::DrawDebugGuI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if(ImGui::Begin("Player",nullptr,ImGuiWindowFlags_None))
	{
		//�g�����X�t�H�[��
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//�ʒu
			ImGui::InputFloat3("Postion", &position.x);

			//��]
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			
			//�X�P�[��
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

//�ړ�����
//void Player::Move(float elapsedTime, float vx, float vz, float speed)
//{
//	speed *= elapsedTime;
//	position.x += vx * speed;
//	position.z += vz * speed;
//}

//�ړ����͏���
bool Player::InputMove(float elapsedTime)
{
	//�i�s�x�N�g���擾
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	Move(moveVec.x, moveVec.z, moveSpeed);

	//���񏈗�
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

	//�i�s�x�N�g�����[���x�N�g���ł͂Ȃ��ꍇ�ɓ��͂��ꂽ
	return (moveVec.x != 0.0f || moveVec.z != 0.0f);


}

//�f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//�Փ˔���p�̃f�o�b�O����`��
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1)); //TODO06�ŉ~���ɕύX

	//�Փ˔���p�̃f�o�b�N�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

	//�e�ۃf�o�b�O�v���~�e�B�u�`��
	projectileManager.DebugPrimitive();

	//�U���Փ˗p�̍���m�[�h�̃f�o�b�O����`��
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

//�v���C���[�ƃG�l�~�[�Փ˔���
void Player::CollisionPlayerEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//�v���C���[vs�G�l�~�[�Փˏ���
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//�Փ˔���
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersecCylinderVsCylinder(
			Player::position,Player::radius,Player::height,
			enemy->GetPosition(),enemy->GetRadius(),enemy->GetHeight(),
			outPosition
		))
		{
			//�G�̐^��ɓ���������
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, N);
			//�ォ�瓥��Â����ꍇ�͏��W��������
			if (normal.y > 0.8f)
			{
				//���W��������
				Jump(jumpSpeed * 0.5f);
			}
			else
			{
				//�����o����̈ʒu�ݒ�
				enemy->SetPosition(outPosition);
			}
		}
	}
}


//�e�ۂƓG�̏Փˏ���
void Player::CollisionProjectileVsEnemies()
{

	EnemyManager& enemyManager = EnemyManager::Instance();

	//�S�Ă̒e�ۂƑS�Ă̓G�𑍓�����ŏՓˏ���
	int projectileCount = projectileManager.GetProjectileCount();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Projectile* projectile = projectileManager.GetProjectile(i);
		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* enemy = enemyManager.GetEnemy(j);

			//�Փˏ���
			DirectX::XMFLOAT3 outPosition;
			if (Collision::InstarsecSphereVsCylinder(
				projectile->GetPosition(), projectile->GetRadius(),
				enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(),
				outPosition
			))
			{
				// �_���[�W��^����
				if (enemy->ApplyDamage(1, 0.5f))
				{
					// �e�۔j��
					projectile->Destroy();
					// ������΂�
					{
						DirectX::XMFLOAT3 direction;
						DirectX::XMFLOAT3 impulse;

						// ������΂��������v�Z
						DirectX::XMFLOAT3 position1 = enemy->GetPosition();
						DirectX::XMFLOAT3 position2 = outPosition;

						DirectX::XMVECTOR vector1 = DirectX::XMLoadFloat3(&position1);
						DirectX::XMVECTOR vector2 = DirectX::XMLoadFloat3(&position2);

						// 2�̃x�N�g���̍����v�Z
						DirectX::XMVECTOR differenceVector = DirectX::XMVectorSubtract(vector1, vector2);

						// ���ʂ�XMFLOAT3�^�ɕϊ����Ċi�[
						DirectX::XMStoreFloat3(&direction, differenceVector);

						// ������΂������𐳋K��
						DirectX::XMVECTOR normalizedDirectionVector = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction));


						float force = 10.0f;
						DirectX::XMVECTOR forceVector = DirectX::XMVectorScale(normalizedDirectionVector, force);

						DirectX::XMStoreFloat3(&impulse, forceVector);

						enemy->AddImpulse(impulse);
						DirectX::XMFLOAT3 e = enemy->GetPosition();
						e.y += enemy->GetHeight() * 0.5f;
						hitEffect->Play(e);
					}
					//�q�b�g�G�t�F�N�g�Đ�
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

//�m�[�h�ƓG�̏Փˏ���
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
	//�m�[�h�擾
	Model::Node* node = model->FindNode(nodeName);

	//�m�[�h�ʒu�擾
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
	
	//�w��̃m�[�h�ƑS�Ă̓G�𑍓�����ŏՓˏ���
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyConut = enemyManager.GetEnemyCount();
	for (int i=0; i < enemyConut; i++)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		//�Փ˔���
		DirectX::XMFLOAT3 outPosition;
		if (Collision::InstarsecSphereVsCylinder(
			nodePosition, nodeRadius,
			enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(),
			outPosition
		))
		{ 
		//�_���[�W��^����
			if (enemy->ApplyDamage(1, 0.5f))
			{
				//������΂�
				{
					DirectX::XMFLOAT3 direction;
					DirectX::XMFLOAT3 impulse;

					//������΂��������v�Z
					DirectX::XMFLOAT3 enemyPosition = enemy->GetPosition();
					DirectX::XMFLOAT3 outPos = outPosition;

					DirectX::XMVECTOR enemyVec = DirectX::XMLoadFloat3(&enemyPosition);
					DirectX::XMVECTOR outVec = DirectX::XMLoadFloat3(&outPos);

					//2�̃x�N�g���̍����v�Z
					DirectX::XMVECTOR differenceVector = DirectX::XMVectorSubtract(enemyVec, outVec);

					//���ʂ�ZMFLOAT�^�ɕϊ����Ċi�[
					DirectX::XMStoreFloat3(&direction, differenceVector);

					//������΂������𐳋K��
					DirectX::XMVECTOR norma = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction));

					float force = 10.0f;
					DirectX::XMVECTOR forceVec = DirectX::XMVectorScale(norma, force);

					DirectX::XMStoreFloat3(&impulse, forceVec);

					enemy->AddImpulse(impulse);
				}
				//�q�b�g�G�t�F�N�g�Đ�
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

	//�����e�۔���
	if (gamepad.GetButtonDown() & GamePad::BTN_X)
	{
		// �v���C���[�̌������擾
		DirectX::XMFLOAT3 playerForward;
		DirectX::XMVECTOR forwardVector = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMQuaternionRotationRollPitchYaw(Player::GetAngle().x, Player::GetAngle().y, Player::GetAngle().z));
		DirectX::XMStoreFloat3(&playerForward, forwardVector);
		// ���˕������v�Z
		DirectX::XMFLOAT3 dir = playerForward; // �v���C���[�̌��������˕���


		// ���ˈʒu���v���C���[�̈ʒu�ɐݒ�
		DirectX::XMFLOAT3 pos;//�v���C���[�̈ʒu�����ˈʒu
		pos.x = GetPosition().x;
		pos.y = GetPosition().y + 0.5f;
		pos.z = GetPosition().z;

		//����
		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
		//projectileManager.Register(projectile); �e�ۃN���X�̃R���X�g���N�^�ŌĂяo��������̂ŕs�v
	}
	
	if (gamepad.GetButtonDown() & GamePad::BTN_Y)
	{
		// �v���C���[�̌������擾
		DirectX::XMFLOAT3 playerForward;
		DirectX::XMVECTOR forwardVector = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMQuaternionRotationRollPitchYaw(Player::GetAngle().x, Player::GetAngle().y, Player::GetAngle().z));
		DirectX::XMStoreFloat3(&playerForward, forwardVector);
		// ���˕������v�Z
		DirectX::XMFLOAT3 dir = playerForward; // �v���C���[�̌��������˕���

		// ���ˈʒu���v���C���[�̈ʒu�ɐݒ�
		DirectX::XMFLOAT3 pos;//�v���C���[�̈ʒu�����ˈʒu
		pos.x = GetPosition().x;
		pos.y = GetPosition().y + 0.5f;
		pos.z = GetPosition().z;

		DirectX::XMFLOAT3 target;
		target.x = position.x + dir.x * 100.0f;
		target.y = position.y + dir.y * 100.0f;
		target.z = position.z + dir.z * 100.0f;

		//��ԋ߂��̃G�l�~�[���^�[�Q�e�B���O
		float dist = FLT_MAX;
		EnemyManager& enemyManager = EnemyManager::Instance();
		int enemyCount = enemyManager.GetEnemyCount();
		for (int i = 0; i < enemyCount; i++)
		{
			//�G�Ƃ̋�������
			Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);            //�v���C���[��position
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());//�G�l�~�[��position
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);             //�G�l�~�[�ƃv���C���[��position���x�N�g����
			DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
			float d;//direction
			DirectX::XMStoreFloat(&d, D);

			if (d < dist)
			{
				dist = d;
				target = enemy->GetPosition();
				target.y += enemy->GetHeight() * 0.5f;
			}

			//����
			ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
			projectile->Launch(dir, pos, target);

		}
	}
}


DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //   ���͏����擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //   �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //   �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���
    //   �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength >  0.0f)
    {
        //  �P�ʃx�N�g����
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    //   �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z; //  Z�������擾
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength >  0.0f)
    {
        //  �P�ʃx�N�g����
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //   �X�e�B�b�N�̐������͒lx���J�����E�����ɔ��f���A
    //   �X�e�B�b�N�̐������͒lz���J�����O�����ɔ��f���A
    //   �i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec3;
	vec3.x = cameraRightX * ax + cameraFrontX * ay ;
    vec3.z = cameraRightZ * ax + cameraFrontZ * ay;

    //  y�������ɂ͈ړ����Ȃ�
    vec3.y =  0.0f;

    return vec3;
}

bool Player::InputJump()
{
	//�W�����v��������
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		if (jumpCount < jumpLimit)
		{
			Jump(jumpSpeed);
			jumpCount++;
			//�W�����v���͂�����
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

//���n�������ɌĂ΂��
void Player::OnLanding()
{
	//���n�������ɃW�����v�񐔂����Z�b�g
	jumpCount = 0;

	//�������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��
	if (velocity.y != 0.0f)
	{
		TransitionLamdState();
	}
}

//�ҋ@�X�e�[�g�֑J��
void Player::TransitionIdleState()
{
	state = State::Idle;

	//�ҋ@�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Idie, true);
}

//�ҋ@�X�e�[�g�X�V����
void Player::UpdateIdleState(float elapsedTime)
{
	//�ړ����͏���
	if (InputMove(elapsedTime))
	{
		TransitionMoveState();
	}
	//�W�����v���͏���
	if (InputJump())
	{
		TranstionJumpState();
	}

	//�U�����͏���
	if (InputArrack())
	{
		TransitionAttackState();
	}

	//�e�ۓ��͏���
	InputProjectile();
}

//�ړ��X�e�[�g�֑J��
void Player::TransitionMoveState()
{
	state = State::Move;

	//���胂�[�V�����Đ�
	model->PlayAnimation(Anim_Running, true);
}

//�ړ��X�e�[�g�X�V����
void Player::UpdateMoveState(float elapsedTime)
{
	//�ړ������͏���
	if (!InputMove(elapsedTime))
	{
		TransitionIdleState();
	}

	//�W�����v���͏���
	if (InputJump())
	{
		TranstionJumpState();
	}

	//�U�����͏���
	if (InputArrack())
	{
		TransitionAttackState();
	}

	//�e�ۓ��͏���
	InputProjectile();
}

//�W�����v�X�e�[�g�ɑJ��
void Player::TranstionJumpState()
{
	state = State::Jump;

	//�W�����v�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Jump, false);
}

//�W�����v�X�e�[�g�X�V����
void Player::UpdateJumpState(float elapsedTime)
{
	//�ړ����͏���
	if (!InputMove(elapsedTime))
	{
		return;
	}

	//�W�����v���͏���
	if (InputJump())
	{
		TranstionJumpState();
	}
	
}

//���n�X�e�[�g�ɑJ��
void Player::TransitionLamdState()
{
	state = State::Land;

	//���n�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Landing, false);
}

//���n�X�e�[�g�X�V����
void Player::UpdateLandState(float elapsedTime)
{
	//���n��,�ړ����͂������ꍇ�ɑ��̏�Ԃւ̑J�ڂ��s��
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}
	//�e�ۓ��͏���
	InputProjectile();
}

//�U���X�e�[�g�ɑJ��
void Player::TransitionAttackState()
{
	state = State::Attack;

	//�U���A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Attack, false);
}

//�U���X�e�[�g�X�V����
void Player::UpdateAttackState(float elapsedTime)
{
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}

	//�U���{�^���������ꂽ��
	if (InputArrack() && !model->IsPlayAnimation())
	{
		TransitionAttackState();
	}

	//�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentAnimationSeconds();
	attackCollisionFlag = (animationTime >= 0.3f && animationTime <= 0.4f);
	
	if (attackCollisionFlag)
	{
		//����m�[�h�ƃG�l�~�[�̏Փˏ���
		CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
		
	}


}
