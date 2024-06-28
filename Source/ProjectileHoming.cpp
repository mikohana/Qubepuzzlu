#include "PrjectileHoming.h"

ProjectileHoming::ProjectileHoming(ProjectileManager* manager) : Projectile(manager)
{
	model = new Model("Data/Model/Sword/Sword.mdl");

	//�X�P�[�����O
	scale.x = scale.y = scale.z = 3.0f;

	moveSpeed = 0.05f;
}

ProjectileHoming::~ProjectileHoming()
{
	delete model;
}

void ProjectileHoming::Update(float elapsedTime)
{
	//��������
	lifeTimer -= elapsedTime;
	if (lifeTimer <= 0.0f)
	{
		Destroy();
	}
	//�ړ�
	{
		float moveSpeed = this->moveSpeed * elapsedTime;
		position.x += direction.x * this->moveSpeed;
		position.y += direction.y * this->moveSpeed;
		position.z += direction.z * this->moveSpeed;
	}
	//����
	{
		float turnSpeed = this->turnSpeed * elapsedTime;
		//�^�[�Q�b�g�܂ł̃x�N�g�����Z�o
	
		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
		DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

		//�[���x�N�g���łȂ��Ȃ��]����
		DirectX::XMVECTOR LengtSq = DirectX::XMVector3Length(Vec);
		float lengthSq;
		DirectX::XMStoreFloat(&lengthSq, LengtSq);
		if (lengthSq > 0.00001f)
		{
			//�^�[�Q�b�g�܂ł̃x�N�g����P�ʃx�N�g����
			Vec = DirectX::XMVector3Normalize(Vec);

			//�����Ă�������x�N�g�����Z�o
			DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);

			//�O�������x�N�g���ƃ^�[�Q�b�g�܂ł̃x�N�g���̓���(�p�x)���Z�o
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);

			float dot;
			DirectX::XMStoreFloat(&dot, Dot);

			//�Q�̒P�ʃx�N�g���̊p�x���������ق�
			//1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐߂���
			float rot = 1.0f - dot;
			
			if (rot > turnSpeed) {
				rot = turnSpeed;
			}

			//��]����肭�����Ȃ������͉��L
			//��]�p�x������Ȃ��]����������
			if (fabsf(rot)>0.0001f)
			{
				//��]�����Z�o
				DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);

				//��]���Ɖ�]�ʂ����]�s����Z�o
				DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);

				//���݂̍s�����]������
				DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&tranceform);
				Transform = DirectX::XMMatrixMultiply(Transform, Rotation);

				//��]��̑O�����������o���A�P�ʃx�N�g��������
				Direction = DirectX::XMVector3Normalize(Transform.r[2]);
				DirectX::XMStoreFloat3(&direction,Direction); // �O�������𐳋K��
			}
		}
	}
	//�I�u�W�F�N�g�s����X�V
	UpdateTranceform();

	//���f���s��X�V
	model->UpdateTransform(tranceform);
}

void ProjectileHoming::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}
//����
void ProjectileHoming::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& traget)
{
	this->direction = direction;
	this->position = position;
	this->target = target;

	UpdateTranceform();
}
