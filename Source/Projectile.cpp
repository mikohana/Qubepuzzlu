#include "Projectile.h"
#include "ProjectilrManager.h"
//�f�o�b�O�v���~�e�B�u�`��


Projectile::Projectile(ProjectileManager* manager) : manager(manager)
{
	//�������Ƀ}�l�[�W���[�𐶐�����
	manager->Register(this);
}

void Projectile::Destroy()
{
	//�}�l�[�W���[���玩�g���폜����
	manager->Remove(this);
}

void Projectile::DebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	
}

//�s��X�V����
void Projectile::UpdateTranceform()
{
	DirectX::XMVECTOR Up, Right, Front;
	// �O�x�N�g�����Z�o
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);

	//���̏�x�N�g����p��
	Up = DirectX::XMVectorSet(0.001f, 1, 0, 0);
	Up = DirectX::XMVector3Normalize(Up);

	// �E�x�N�g�����Z�o
	Right = DirectX::XMVector3Cross(Up, Front);
	Right = DirectX::XMVector3Normalize(Right);

	// ��x�N�g�����Z�o
	// Right��Front�̊O�ς���邱�Ƃŏ�x�N�g�����v�Z
	Up = DirectX::XMVector3Cross(Front,Right);

	// �v�Z�������o��
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	//�Z�o����������x�N�g���s����쐬
	tranceform._11 = right.x * scale.x;
	tranceform._12 = right.y * scale.x;
	tranceform._13 = right.z * scale.x;
	tranceform._14 =0.0f;
	tranceform._21 = up.x * scale.x;
	tranceform._22 = up.y * scale.y;
	tranceform._23 = up.z * scale.z;
	tranceform._24 =0.0f;
	tranceform._31 = front.x * scale.z;
	tranceform._32 = front.y * scale.z;
	tranceform._33 = front.z * scale.z;
	tranceform._34 =0.0f;
	tranceform._41 = position.x;
	tranceform._42 = position.y;
	tranceform._43 = position.z;
	tranceform._44 =1.0f;

	//���˕���
	this->direction = front;
	
}

