#include "StageMoveFloor.h"

//�R���X�g���N�^
StageMoveFloor::StageMoveFloor()
{
	scale.x = scale.z = 3.0f;
	scale.y = 0.5f;

	//�X�e�[�W���f����ǂݍ���
	model = new Model("Data/Model/Cube/Cube.mdl");
}

StageMoveFloor::~StageMoveFloor()
{
	//�X�e�[�W���f����j��
	delete model;
}

//�X�V����
void StageMoveFloor::Update(float elapsedTime)
{
	//�O��̏���ۑ� 
	oldTransform = transform;
	oldAngle = angle;

	//�X�e�[�W����S�[���܂ł̋������Z�o����
	DirectX::XMVECTOR Start  =DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR Goal   =DirectX::XMLoadFloat3(&goal);
	DirectX::XMVECTOR Vec    =DirectX::XMVectorSubtract(Goal, Start);
	DirectX::XMVECTOR Length =DirectX::XMVector3Length(Vec);
	float lenght;
	DirectX::XMStoreFloat(&lenght, Length);

	//�X�^�[�g����S�[���܂ł̊Ԃ���b�Ői�ފ���(0.0�`1.0)���Z�o����
	float speed = moveSpeed * elapsedTime;
	float speedRate = speed / lenght;
	moveRate += speedRate;
	//�S�[���ɓ��B�A�܂��̓X�^�[�g�ɖ߂����ꍇ�A�ړ������𔽓]������
	if (moveRate <= 0.0f || moveRate >= 1.0f)
	{
		moveSpeed = -moveSpeed;
	}
	//���`�ۊǂňʒu���Z�o����
	DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
	DirectX::XMStoreFloat3(&position, Position);

	//��]
	angle.x += torque.x * elapsedTime;
	angle.y += torque.y * elapsedTime;
	angle.z += torque.z * elapsedTime;

	//�s��X�V
	UpdateTransform();

	////���f���s��X�V
	//model->UpdateTransform(transform);

	//���C�L���X�g�p�ɂɃ��f����ԕ\��ɂ��邽�߂ɒP�ʍs���n��
	const DirectX::XMFLOAT4X4 transformIdentity = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	model->UpdateTransform(transformIdentity);
}

//�`�揈��
void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	//�\���p�̂��߃��[���h�s��ɍX�V����
	model->UpdateTransform(transform);
	shader->Draw(dc, model);
}

//���C�L���X�g
bool StageMoveFloor::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	//�O��̃��[���h�s��Ƌt�s�񉻂���
	DirectX::XMMATRIX WorldTransform = DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&oldTransform));

	//�O��̃��[�J����ԂŃ��C�ɕϊ�
	DirectX::XMVECTOR RayStart = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR RayEnd = DirectX::XMLoadFloat3(&end);
	RayStart = DirectX::XMVector3TransformCoord(RayStart, WorldTransform);
	RayEnd = DirectX::XMVector3TransformCoord(RayEnd, WorldTransform);
	//���[�J����Ԃł̃��C�Ƃ̌�_�����߂�
	DirectX::XMFLOAT3 localStart, localEnd;
	DirectX::XMStoreFloat3(&localStart, RayStart);
	DirectX::XMStoreFloat3(&localEnd, RayEnd);

	HitResult localHit;
	if (Collision::IntersecRayVsModel(localStart, localEnd, model, localHit))
	{
		//�O��̃��[�J����Ԃ��獡��̃��[���h��Ԃ֕ϊ�
		//DirectX::XMStoreFloat4x4(&transform, oldInverseTransform);
		
		
		DirectX::XMVECTOR hitPosint = DirectX::XMLoadFloat3(&localHit.position);
		hitPosint = DirectX::XMVector3TransformCoord(hitPosint, DirectX::XMLoadFloat4x4(&transform));
		
		//�ϊ����ꂽ��_���i�[
		DirectX::XMStoreFloat3(&hit.position, hitPosint);
		hit.rotation.x =- oldAngle.x + angle.x;
		hit.rotation.y =- oldAngle.y + angle.y;
		hit.rotation.z =- oldAngle.z + angle.z;

		//�Փˏ���ݒ�
		hit.normal = localHit.normal;
		hit.distance = localHit.distance;

	
		return true;
	}
	return false;
}

//�s��X�V����
void StageMoveFloor::UpdateTransform()
{
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, W);
}
