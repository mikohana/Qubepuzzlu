#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectilrManager.h"
#include "Effect.h"

//�v���C���[
class Player : public Character
{
public:
	Player();
	~Player() override;

	//�C���X�^���X�擾
	static Player& Instance();

	//�X�V����
	void Update(float elapedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//�f�o�b�O�pGUI�`�揈��
	void DrawDebugGuI();

	//�����蔻��
	void DrawDebugPrimitive();

	//�v���C���[�ƃG�l�~�[�Ƃ̏Փ˔���
	void CollisionPlayerEnemies();

	

	//�e�ۂƓG�̏Փˏ���
	void CollisionProjectileVsEnemies();

	//�m�[�h�ƃG�l�~�[�̏Փ˔���
	void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

	//�e�ۓ��͏���
	void InputProjectile();

	
protected:

	//���n����
	void OnLanding() override;

private:
	//�A�j���[�V����
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

	//�C���X�^���X�擾
private:
	//�X�e�B�b�N�̓��͒l����ړ��x�N�g�����擾
	DirectX::XMFLOAT3 GetMoveVec() const;
	//�ړ����͏���
	bool  InputMove(float elapsedTime);

	//�W�����v���͏���
	bool InputJump();

	//�U�����͏���
	bool InputArrack();

	//�ҋ@�X�e�[�g�֑J��
	void TransitionIdleState();

	//�ҋ@�X�e�[�g�X�V����
	void UpdateIdleState(float elapsedTime);

	//�ړ��X�e�[�g�֑J��
	void TransitionMoveState();

	//�ړ��X�e�[�g�X�V����
	void UpdateMoveState(float elapsedTime);

	//�W�����v�X�e�[�g�ɑJ��
	void TranstionJumpState();

	//�W�����v�X�e�[�g�X�V����
	void UpdateJumpState(float elapsedTime);

	//���n�X�e�[�g�ɑJ��
	void TransitionLamdState();

	//���n�X�e�[�g�X�V����
	void UpdateLandState(float elapsedTime);

	//�U���X�e�[�g�֑J��
	void TransitionAttackState();

	//�U���X�e�[�g�X�V����
	void UpdateAttackState(float elapsedTime);
	
private:
	//�X�e�[�g
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