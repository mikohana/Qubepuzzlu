#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
void CameraController::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	//�J�����̉�]���x
	float speed = rollSpeed * elapsedTime;

	//  �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
	angle.x += ay * speed; // X���̉�]
	angle.y += ax * speed; // Y���̉�]

	// X���̃J������]�𐧌�
	const float maxPitch = DirectX::XM_PI / 4.0f;
	const float minPitch = -maxPitch;

	//  �s�b�`���ő�l�𒴂����ꍇ�́A�ő�l�ɐ���
	if (angle.x > maxPitch)
	{
		angle.x = maxPitch;
	}
	//  �s�b�`���ŏ��l����������ꍇ�́A�ŏ��l�ɐ���
	else if (angle.x < minPitch)
	{
		angle.x = minPitch;
	}

	// Y���̉�]����-3.14�`3.14�Ɏ��܂�悤�ɂ���
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	//�J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//�����_������̃x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	
	eye.x = this->target.x - front.x * this->range;
	eye.y = this->target.y - front.y * this->range;
	eye.z = this->target.z - front.z * this->range;
	//�J�����̎��_�ƒ����_��ݒ�
	Camera::Instance().SetLoolAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}
