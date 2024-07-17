#include "Camera.h"
#if true

void Camera::SetLoolAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
	//���_�A�����_�A�㎋�_����r���[�s����쐬
	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);    //�J�����̈ʒu��\���x�N�g��
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);//�J�������������Ă���_��\���x�N�g��
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);      //�J�����̏�����̃x�N�g��
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	DirectX::XMStoreFloat4x4(&view, View);


	//�r���[���t�s�񉻂��A���[���h�\��ɖ߂�
	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, World);

	//�J�����̕��������o��
	this->right.x = world._11;
	this->right.y = world._12;
	this->right.z = world._13;

	this->up.x = world._21;
	this->up.y = world._22;
	this->up.z = world._23;

	this->front.x = world._31;
	this->front.y = world._32;
	this->front.z = world._33;

	//���_�A�����_��ۑ�
	this->eye = eye;
	this->focus = focus;
}
#endif // false
#if  false

void Camera::SetLoolAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
	// �N�I�[�^�j�I����XMMATRIX�ɕϊ�
	DirectX::XMMATRIX quaternionMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&this->));

	// �N�I�[�^�j�I���ɂ���]�𔽉f������
	DirectX::XMVECTOR Eye = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&eye), quaternionMatrix);
	DirectX::XMVECTOR Focus = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&focus), quaternionMatrix);
	DirectX::XMVECTOR Up = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&up), quaternionMatrix);

	// ���_�A�����_�A�㎋�_����r���[�s����쐬
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	DirectX::XMStoreFloat4x4(&view, View);

	// �r���[���t�s�񉻂��A���[���h�\��ɖ߂�
	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, World);

	// �J�����̕��������o��
	this->right.x = world._11;
	this->right.y = world._12;
	this->right.z = world._13;

	this->up.x = world._21;
	this->up.y = world._22;
	this->up.z = world._23;

	this->front.x = world._31;
	this->front.y = world._32;
	this->front.z = world._33;

	// ���_�A�����_��ۑ�
	this->eye = eye;
	this->focus = focus;
}

#endif //  false

//�p�[�X�y�N�e�B�u�ݒ�
void Camera::SetPerspectiveFor(float fovY, float aspect, float nearZ, float farZ)
{
	//��p�A��ʔ䗦�A�N���b�v��������v���W�F�N�V�����s����쐬
	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);//LH�͍�����W�Ƃ����Ӗ��AL��Left,H��Hand
	DirectX::XMStoreFloat4x4(&projection, Projection);
}

//
////�N�I�[�^�j�I���X�V����
//void Camera::UpdateQuaternion(const DirectX::XMFLOAT3& axis, float angle)
//{
//	// �����̃N�I�[�^�j�I����ێ�
//	DirectX::XMFLOAT4 oldQuaternion = this->quaternion;
//
//	// �p�x�����W�A���ɕϊ�
//	float radius = DirectX::XMConvertToRadians(angle);
//
//	// �N�I�[�^�j�I���𐶐�
//	DirectX::XMFLOAT4 newQuaternion;
//	newQuaternion.w = cos(radius / 2);
//	newQuaternion.x = axis.x * sin(radius / 2);
//	newQuaternion.y = axis.y * sin(radius / 2);
//	newQuaternion.z = axis.z * sin(radius / 2);
//
//	// oldQuaternion��newQuaternion��DirectX::XMFLOAT4�^�ł���ꍇ�̏���
//	DirectX::XMVECTOR oldQuaternionVector = DirectX::XMLoadFloat4(&oldQuaternion);
//	DirectX::XMVECTOR newQuaternionVector = DirectX::XMLoadFloat4(&newQuaternion);
//
//	DirectX::XMVECTOR resultQuaternionVector = DirectX::XMQuaternionMultiply(oldQuaternionVector, newQuaternionVector);
//	// ���ʂ�DirectX::XMFLOAT4�ɕϊ�
//	DirectX::XMFLOAT4 resultQuaternion;
//	DirectX::XMStoreFloat4(&resultQuaternion, resultQuaternionVector);
//
//	// �V�����N�I�[�^�j�I����ۑ�
//	this->quaternion = resultQuaternion;
//
//	// �N�I�[�^�j�I����XMMATRIX�ɕϊ�
//	DirectX::XMMATRIX quaternionMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&this->quaternion));
//
//	// �N�I�[�^�j�I���ɂ���]�𔽉f������
//	DirectX::XMStoreFloat3(&this->eye, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&this->eye), quaternionMatrix));
//	DirectX::XMStoreFloat3(&this->focus, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&this->focus), quaternionMatrix));
//}