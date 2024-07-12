#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

//�X�e�[�W
class StageMain : public Stage
{
public:
	StageMain();
	~StageMain() override;

	//�X�V����
	void Update(float elapedTime) override;
	//�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	
	//���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;
	
	//�X�e�[�W�̉�]�X�V����
	void UpdateStageRotate(float elapsedTime);
	


public:
	Model* model = nullptr;
	float rotateSpeed = 0.3f;   //��]���x(�x/�b)
	float totalRotation = 0.0f;
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	bool isRotating = false;
	DirectX::XMFLOAT3 rotation = { 0,0,0 };


	DirectX::XMFLOAT4X4 stage_transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
};