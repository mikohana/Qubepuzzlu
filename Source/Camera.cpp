#include "Camera.h"

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

//�p�[�X�y�N�e�B�u�ݒ�
void Camera::SetPerspectiveFor(float fovY, float aspect, float nearZ, float farZ)
{
	//��p�A��ʔ䗦�A�N���b�v��������v���W�F�N�V�����s����쐬
	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);//LH�͍�����W�Ƃ����Ӗ��AL��Left,H��Hand
	DirectX::XMStoreFloat4x4(&projection, Projection);
}
