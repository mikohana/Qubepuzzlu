#pragma once

#include <DirectXMath.h>

//�L�����N�^�[
class Character
{
public:
	Character() {}
	virtual ~Character() {}

	//�s��X�V����
	void UpdateTransform();

	//�ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	//�ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	//��]�擾
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	//��]�ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	//�X�P�[���擾
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	//�X�P�[���ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	//���a�擾
	float GetRadius() const { return radius; }

	//���N��Ԃ��擾
	int GetHealth() const { return health; }

	//�ő匒�N��Ԃ��擾
	int GetMaxHealth() const { return maxHealth; }

	//�n�ʂɐڒn���Ă��邩
	bool IsGround() const { return isGround; }

	//�����擾
	float GetHeight() const { return height; }

	//�_���[�W��^����
	bool ApplyDamage(int damage, float incincibleTime);//���G���ԗp�̈�����ǉ�

	//�Ռ���^����
	void AddImpulse(const DirectX::XMFLOAT3& impilse); 
	
protected:

	//�_���[�W���󂯂����ɌĂ΂��
	virtual void OrDamage() {};

	//���S�������ɌĂ΂��
	virtual void OnDead() {}

	//���n������Ă΂��
	virtual void OnLanding() {}//�I�[�o�[���C�h�\

	//���G���ԍX�V
	void UpdateInvincTimer(float elapsedTime);

	//�ړ�����
	// void Move(float elapsedTime, float vx, float vz, float speed);
	void MoveVec(float vx, float vz, float speed);

	//�X�Η��ňړ������x�N�g����ێ�����֐�
	//void SlopeMove(float vx, float vy, float vz, float slopSpeed);

	//���񏈗�
	void Turn(float elaspedTime, float vx, float vz, float speed);

	//�W�����v����
	void Jump(float speed);

	//���͏����X�V
	void UPdateVelocity(float elaspedTime);

private:
	//�������͍X�V����
	void UpdateVerticalVelocity(float elapsedFrame);

	//�����ړ��X�V����
	void UpdateVerticalMove(float elapsedTime);

	//�������͍X�V����
	void UpdateHorizontalVelocity(float elapsedFrame);

	//�����ړ��X�V����
	void UpdateHorizontalMove(float elapsedTime);

	//�J�����̉�]�Ɋ�Â��ăL�����N�^�[�̈ړ��𒲐�����֐�
	void MoveWithCameraRotation(float elapsedTime);

	//Camera��]�p�x��Character�����ړ��X�V����
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

	//�i�s�x�N�g�����v�Z


	DirectX::XMFLOAT3 velocity = { 0,0,0 };
};