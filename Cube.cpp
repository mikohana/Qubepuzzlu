#include "Cube.h"

Cube::Cube(const char* filename)
{
	model = new Model(filename);
}
Cube::~Cube()
{
	delete model;
}

void  Cube::UpdateTransform()
{
	//�X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

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
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);//����������tranceform._11,tranceform._22,tranceform._33���Ƃ��̏�ŉ�]�ɂȂ�

	//3�̍s���g�ݍ��킹���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;

	//�v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&tranceform, W);

	model->UpdateTransform(tranceform);
}

void Cube::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}