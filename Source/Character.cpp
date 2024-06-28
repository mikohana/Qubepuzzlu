#include "Character.h"
#include "Input/Input.h"
#include "Mathf.h"
#include "StageManager.h"

//�s��X�V����
void  Character::UpdateTransform()
{
	//�X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x,scale.y,scale.z);

	//��]�s����쐬
	//DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(this->angle.x,this->angle.y,this->angle.z);
	// X���̉�]�s��
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	// Y���̉�]�s��
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	// Z���̉�]�s��
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);

	//��]�s����쐬
	DirectX::XMMATRIX R = Y * X * Z;

	//�ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x,position.y,position.z);//����������tranceform._11,tranceform._22,tranceform._33���Ƃ��̏�ŉ�]�ɂȂ�

	//3�̍s���g�ݍ��킹���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;

	//�v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&tranceform, W);
}

bool Character::ApplyDamage(int damage, float invincbleTime)
{
	// �_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
	if (damage <= 0) return false;

	// ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
	if (health <= 0) return false;

	// �_���[�W����
	health -= damage;

	// ���S�ʒm
	if (health <= 0)
	{
		OnDead();
	}
	//�_���[�W�ʒm
	else
	{
		OrDamage();
		UpdateInvincTimer(invincbleTime);
	}
		

	//���N��Ԃ��ύX�����ꍇ��true��Ԃ�
	return true; // �_���[�W���K�p���ꂽ���Ƃ�����
}
//�Ռ���^����
void Character::AddImpulse(const DirectX::XMFLOAT3& impilse)
{
	//���͂ɗ͂�������
	velocity.x += impilse.x;
	velocity.y += impilse.y;
	velocity.z += impilse.z;
}
//���G���ԍX�V
void Character::UpdateInvincTimer(float elapsedTime)
{
	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= elapsedTime;
	}
}

//void Character::Move(float elapsedTime, float vx, float vz, float speed)
//{
//	//�ړ������x�N�g����ݒ�
//	moveVecX = vx;
//	moveVecZ = vz;
//
//	//�ő呬�x�ݒ�
//	maxMoveSpeed = speed;
//}

void Character::Move(float vx, float vz, float speed)
{
	//�����x�N�g����ݒ�
	moveVecX = vx;
	moveVecZ = vz;

	//�ő呬�x����
	maxMoveSpeed = speed;
}


void Character::Turn(float elaspedTime, float vx, float vz, float speed)
{
	speed *= elaspedTime;
	// �i�s�x�N�g����0�x�N�g���̏ꍇ��������K�v�Ȃ�
	if (vx == 0.0f && vz == 0.0f)
	{
		return;
	}
	//�i�s�x�N�g����P�ʃx�N�g����
	float lenth = sqrtf(vx * vx + vz * vz);
	vx /= lenth;
	vz /= lenth;

	//�����̉�]�͖����������߂�
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	//�����̉�]�p�����߂邽��2�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = vx * frontX + vz * frontZ;

	//���ϒl��2�̒P�ʃx�N�g���̊p�x���������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐߂���
	float rot = 1.0f - dot;
	if (speed > rot)speed = rot;

	//���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
	float cross = (vz * frontX) - (vx * frontZ);

	//�O�ς��������ō��E�𔻒肷��
	if (cross < 0.0f)
	{
		//�E�̏ꍇ
		angle.y += speed;
	}
	else
	{
		//���̏ꍇ
		angle.y -= speed;
	}
}

void Character::Jump(float speed)
{
	velocity.y = speed;
}

void Character::UPdateVelocity(float elaspedTime)
{
	//�o�߃t���[��
	float elaspedFrame = 60.0f * elaspedTime;

	//�������͍X�V����
	UpdateVerticalVelocity(elaspedFrame);

	//�������͍X�V����
	UpdateHorizontalVelocity(elaspedFrame);

	//�����ړ��X�V����
	UpdateVerticalMove(elaspedTime);

	//�����ړ��X�V����
	UpdateHorizontalMove(elaspedTime);
}

//�������͍X�V����
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
//�d�͏���;
	velocity.y += gravity * elapsedFrame;
}


//�����ړ�����
void Character::UpdateVerticalMove(float elapsedTime)
{
	//���������̈ړ���
	float my = velocity.y * elapsedTime;

	//�L�����N�^�[��Y�������ƂȂ�@���x�N�g��
	DirectX::XMFLOAT3 normal = { 0,1,0 };
	
	//������
	if (my < 0.0f)
	{
		
		//���C�̊J�n�ʒu�͑�����菭����
		DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
		//���C�̏I�_�ʒu�͈ړ���̈ʒu
		DirectX::XMFLOAT3 end = { position.x,position.y + my,position.z };

		//���C�L���X�g�ɂ��n�ʔ���
		HitResult hit;
		
		if(StageManager::Instance().RayCast(start,end,hit))
		{
			//�@���x�N�g���擾
			normal = hit.normal;

			//�n�ʂɐڐG���Ă���
			position = hit.position;
			//��]
			angle.y += hit.rotation.y;
			
			//�X�Η��̌v�Z
			float normallengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
			slopeRate = 1.0f - (hit.normal.y / (normallengthXZ + hit.normal.y));


			//���n������
			if (!isGround)
			{
				OnLanding();
			}
			isGround = true;
			velocity.y = 0.0f;
		}
		else
		{
			//�󒆂ɕ����Ă�����
			position.y += my;
			isGround = false;
		}
	
	
	}
	//�㏸��
	else if (my> 0.0f)
	{
		position.y += my;
		isGround = false;
	}



	//�n�ʂ̌����ɉ����悤��XZ����]
	{
		//Y�����@���x�N�g�������Ɍ����I�C���[�i��]���Z�o����
		/*angle.x = atan2f(normal.z, normal.y);
		angle.z = -atan2f(normal.x, normal.y);*/
		

		float Euler_angleX =  atan2f(normal.z, normal.y);
		float Euler_angleZ = -atan2f(normal.x, normal.y);

		//���`�ۊǂŊ��炩�ɉ�]������
		angle.x = Mathf::Lerp(angle.x, Euler_angleX, 0.2f);
		angle.z = Mathf::Lerp(angle.z, Euler_angleZ, 0.2f);

	}

	
}


//�������͍X�V����
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	// XZ���ʂ̑��x����������
	float length = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// ���C��
		 float friiction = this->friiction * elapsedFrame;

		 //�󒆂ɋ��鎞�͖��C�͂����炷
		 if (!IsGround()) friiction *= (1.0f - airControl); // ���C�͂�30%���炷

		// ���C�͂ɂ�錸������
		if (length > friiction)
		{
			// ���x�̕������v�Z
			float directionX = velocity.x / length;
			float directionZ = velocity.z / length;

			// ���x�̒����Ɍ�������K�p���ĐV�����������v�Z
			//float newLength = length - friiction;

			// �V�������x���v�Z
			velocity.x -= directionX * friiction;
			velocity.z -= directionZ * friiction;
		}
		else
		{
			// ���x�����C�͈ȉ��ɂȂ����ꍇ�͑��x��0�ɐݒ�
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}
	// XZ���ʂ̑��x����������
	if (length <= maxMoveSpeed)
	{
		// �ړ��x�N�g�����[���x�N�g���łȂ��ꍇ�͉�������
		float moveVecLength = sqrt(moveVecX * moveVecX + moveVecZ * moveVecZ); // �ړ��x�N�g���̒������v�Z
		if (moveVecLength > 0.0f)
		{
			// ������
			float acceleration = this->acceleration * elapsedFrame;

			//�����ł����������Ȃ��悤�ɂ���
			if (isGround && slopeRate > 0.0f)
			{
				velocity.y -= length * slopeRate * elapsedFrame;
			}

			//�󒆂ɂ��鎞�͉����͂����炷
			if (!IsGround() )acceleration *= (1.0f - airControl);//�����͂����炷

			// �ړ��x�N�g���ɉ�������
			velocity.x += (moveVecX * acceleration);
			velocity.z += (moveVecZ * acceleration);

			// �ő呬�x����
			float newLength = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
			if (newLength > maxMoveSpeed)
			{
				// ���x���ő呬�x�ɐ���
				float ratio = maxMoveSpeed / newLength;
				velocity.x *= ratio;
				velocity.z *= ratio;
			}
		}
	}

	// �ړ��x�N�g�������Z�b�g
	moveVecX = 0.0f;
	moveVecZ = 0.0f;

}

void Character::UpdateHorizontalMove(float elapsedTime)
{
	// �������͌v�Z
	float velocityLengthXZ = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		// �����ړ��l
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;

		// ���C�̊J�n�ʒu�ƏI�_�ʒu
		DirectX::XMFLOAT3 start = { position.x, position.y + 1.0f, position.z };
		DirectX::XMFLOAT3 end = { position.x + mx, position.y + 1.0f, position.z + mz };

		// ���C�L���X�g�ɂ��ǔ���
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Ray = DirectX::XMVectorSubtract( DirectX::XMLoadFloat3(&hit.position), End);
		    //DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Start, End);

				//�ǂ̖@��
				DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

				//���˃x�N�g����@���Ɏˉe
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Ray, Normal);

				//�␳�ʒu�̌v�Z
				//�@���x�N�g���Ɛi�s�����̃h�b�g�ς��v�Z���āA���蕝(a)�Ƃ��Ďg�p
				float a = DirectX::XMVectorGetX(Dot);


				// ��_����̊����̈ʒu���v�Z
				DirectX::XMVECTOR rVec = DirectX::XMVectorAdd(End, DirectX::XMVectorScale(Normal, a * 1.8f));
				DirectX::XMFLOAT3 R;
				DirectX::XMStoreFloat3(&R, rVec);

				HitResult hitwall;
				//hit.position���J�n�ʒu�Ƃ��āAR���I�_�ʒu�Ƃ��ă��C�L���X�g������
				if (!StageManager::Instance().RayCast(hit.position,R,hitwall))
				{
					//�������ċ��Ȃ�������

				position.x = R.x;
				position.z = R.z;
				}
				else
				{
					//�������ċ�����hitwall.position���ŏI�I�Ȉʒu�Ƃ��Ĕɉh
					position.x = hitwall.position.x;
					position.z = hitwall.position.z;
					
				}
		}
		else
		{
			// �ǂɓ������Ă��Ȃ��ꍇ�A�ʏ�̈ړ�
			position.x += mx;
			position.z += mz;
		}
	}
}