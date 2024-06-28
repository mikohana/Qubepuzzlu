#include "ProjectileStraight.h"
#include "StageManager.h"

//�R���X�g���N�^
ProjectileStraight::ProjectileStraight(ProjectileManager* manager) : Projectile(manager)
{
	//model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
	model = new Model("Data/Model/Sword/Sword.mdl");

	//�\���T�C�Y���X�V
	 scale.x = scale.y = scale.z = 3.0f;
}

ProjectileStraight::~ProjectileStraight()
{
	delete model;
}

void ProjectileStraight::Update(float elapsedTime)
{
	//��������
	 lifetime-= elapsedTime;
	if (lifetime <= 0)
	{
		Destroy();
	}
	//�ړ�
	float speed = this->speed * elapsedTime;
	float mx = direction.x * speed;
	float mz = direction.z * speed;

	//�I�u�W�F�N�g�s��X�V
	UpdateTranceform();

	//���f���s��X�V
	model->UpdateTransform(tranceform);

		//���C�̊J�n�n�_�ƏI�_
		DirectX::XMFLOAT3 start = { position.x, position.y + 1.0f, position.z };
		DirectX::XMFLOAT3 end = { position.x + mx, position.y + 1.0f, position.z + mz };

		//���C�L���X�g�ɂ��ǔ���
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Ray = DirectX::XMVectorSubtract(End,Start);

			//�ǂ̖@��(P)
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			//���˃x�N�g����@���Ɏˉe(V)
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Ray,Normal);

			//�@���x�N�g���Ɛi�s�����̃h�b�g�ς��v�Z���āA���˕��Ƃ��Ďg�p
			float correction = DirectX::XMVectorGetX(Dot);

			//���΃x�N�g��(O)�̌v�Z
			DirectX::XMVECTOR ReflectPos = DirectX::XMVectorSubtract(Ray, DirectX::XMVectorScale(Normal, 2.0f * correction));
			
			// Reflectdir = ReflectPos - hit.position
			DirectX::XMVECTOR HitPostion = DirectX::XMLoadFloat3(&hit.position);
			DirectX::XMVECTOR Reflectdir = DirectX::XMVectorSubtract(ReflectPos, HitPostion);


			// ��_����̔��˂̈ʒu(R)���v�Z
			DirectX::XMStoreFloat3(&direction, ReflectPos);
			
		}
		else
		{
			position.x += mx;
			position.z += mz;
		}
		
	}
	




void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
}
