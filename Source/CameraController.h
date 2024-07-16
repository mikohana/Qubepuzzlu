#pragma once

#include <DirectXMath.h>

//�J�����R���g���[���[
class CameraController
{
public:
	CameraController();
	~CameraController() {};
	//�C���X�^���X�̎擾
	static CameraController& Instance()
	{
		static CameraController instance;
		return instance;
	}

	void Update(float elapedTime);

	//�f�o�b�O�pGUI�`�揈��
	void DrawDebugGuI();

	//�^�[�Q�b�g�ʒu�ݒ�
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

	//��]���擾
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	//�����_�擾
	const DirectX::XMFLOAT3& GetEye() const { return eye; }
private:
	DirectX::XMFLOAT3 target = { 0,0,0 };
	DirectX::XMFLOAT3 angle =  { 0,0,0 };
	DirectX::XMFLOAT3 eye = { 0,0,0 };
	float             rollSpeed = DirectX::XMConvertToRadians(90);
	float             range = 15.0f;

	float rotateSpeed = 1.0f;

	float maxAngle = DirectX::XMConvertToRadians(45);
	float minAngle = DirectX::XMConvertToRadians(-45);
};