#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//�X���C��
class  EnemySlime : public Enemy
{
public:
	 EnemySlime ();
	~EnemySlime() override;

	void Update(float elapsedTime) override;

	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//�f�o�b�O�v���~�e�B�u
	void DrawDebugPrimitive() override;

	//�꒣��ݒ�
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

private:
	//�m�[�h�ƃv���C���[�̏Փˏ���
	void CollisionNodeVsPlayer(const char* nodeName, float boneRadius);

	//�^�[�Q�b�g�ʒu�������_���ɐݒ�
	void SetRandomTargetPosition();

	//�v���C���[���G
	bool SartchPlayer();

	//�ڕW�n�_�ֈړ�
	void MoveToTarget(float elapsedTime, float speedRate);

	//�p�j�X�e�[�g�֑J��
	void TransitionWanderState();

	//�p�j�X�e�[�g�X�V���u
	void UpdateWanderState(float elapsedTime);

	//�ҋ@�X�e�[�g�ɑJ��
	void TransitionIdleState();

	//�ҋ@�X�e�[�g�X�V����
	void UpdateIdleState(float elapsedTime);

	//�ǐՃX�e�[�g�֑J��
	void TransitionPursuitState();

	//�ǐՃX�e�[�g�X�V����
	void UpdatePursuitState(float elapsedTime);

	//�U���X�e�[�g�֑J��
	void TransitionAttackState();

	//�U���X�e�[�g�X�V����
	void UpdateAttackState(float elapsedTime);

	//�_���[�W�X�e�[�g�֑J��
	void TransitionDamageState();

	//�_���[�W�X�e�[�g�X�V����
	void UpdateDamageState(float elapsedTime);

	//���S�X�e�[�g�֑J��
	void TransitionDeathState();

	//���S�X�e�[�g�X�V����
	void UpdateDeathState();

private:
	//�X�e�[�g
	enum  class State
	{
		Wander,
		idle,
		Pursuit,
		Attack,
		Damage,
		Death
	};
	//�A�j���[�V����
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
	//���S�������ɌĂ΂��
	void OnDead() override;

	//�_���[�W���󂯂��Ƃ��ɌĂ΂��
	//void OnDamaged() override;
};

 