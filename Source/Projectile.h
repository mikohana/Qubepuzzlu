#pragma once

#include "Graphics/Graphics.h"
//���݃C���N���[�h����Ȃ��悤�ɑO���錾����
class ProjectileManager;

//�e��
class Projectile
{
public:
	//Projectile() {}
	Projectile(ProjectileManager* manager);
	virtual ~Projectile() {}

		//�X�V����
		virtual void Update(float elaspedTime) = 0;

		//�`�揈��
		virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

		//�j��
		void Destroy();

		//�f�o�b�O�v���~�e�B�u�`��
		virtual void DebugPrimitive();

		//�ʒu�擾
		const DirectX::XMFLOAT3& GetPosition() const { return position; };

		//�����擾
		const DirectX::XMFLOAT3& GetDirection() const { return direction; };

		//�X�P�[���擾
		const DirectX::XMFLOAT3& GetScale() const { return scale; };

		//���a�擾
		float GetRadius() const { return radius; }
	
protected:
	//�s��X�V����
	void UpdateTranceform();

	ProjectileManager* manager = nullptr;

protected:
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 direction = { 0,0,1 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT3 velocity = { 0,0,0 };
	DirectX::XMFLOAT4X4 tranceform = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1};
	float radius = 0.5f;
};