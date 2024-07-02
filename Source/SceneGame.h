#pragma once
#include "Stage.h"
#include "Player.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "CameraController.h"
//�r���[�|�[�g�͈̔͂̍\����
struct  Viewport
{
	float x;
	float y;
	float Width;
	float Height;
	float MinZ;
	float MaxZ;
};


// �Q�[���V�[��
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override{};

	// ������
	void Initialize() override;

	// �I����
	void Finalize();

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

private:
	//�G�l�~�[HP�Q�[�W�`��
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	//���[���h���W����X�N���[�����W�ւ̕ϊ����s���֐�
	DirectX::XMFLOAT2 WorldToScreen(const DirectX::XMFLOAT3& worldPos, const Viewport& viewport, const DirectX::FXMMATRIX projection, const DirectX::FXMMATRIX& view, const DirectX::FXMMATRIX world)
	{
		//�X�N���[�����W���v�Z
		DirectX::XMVECTOR screenPosVec = DirectX::XMVector3Project(
			DirectX::XMLoadFloat3(&worldPos),
			viewport.x,
			viewport.y,
			viewport.Width,
			viewport.Height,
			viewport.MinZ,
			viewport.MaxZ,
			projection,
			view,
			world
		);
		
		//XMVECTOR����XMFLOAT2�ɕϊ�
		DirectX::XMFLOAT2 screenPos;
		screenPos.x = DirectX::XMVectorGetX(screenPosVec);
		screenPos.y = DirectX::XMVectorGetY(screenPosVec);
		return screenPos;
	};

	//�X�N���[�����W���烏�[���h���W�ւ̕ϊ�������֐�
	DirectX::XMFLOAT3 ScreenToWorld(float x,float y,float z,const Viewport& viewport,const DirectX::FXMMATRIX projectile,const DirectX::FXMMATRIX view,const DirectX::FXMMATRIX world)
	{
		//���[���h���W���v�Z
		DirectX::XMVECTOR worldPosVec = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z)),
			viewport.x,
			viewport.y,
			viewport.Width,
			viewport.Height,
			viewport.MinZ,
			viewport.MaxZ,
			projectile,
			view,
			world
		);

		//XMVECTOR����XMFLOAT3�ɕϊ�
		DirectX::XMFLOAT3 worldPos;
		DirectX::XMStoreFloat3(&worldPos, worldPosVec);

		return worldPos;
	};

private:
	Stage* stage = nullptr;

	Player* player = nullptr;

	Sprite* gauge = nullptr;

	CameraController* cameraController = nullptr;

	int clickCount = 0;
	int textureWidth; // �e�N�X�`���̕�
	int gaugeWidth;   // ���݂̃Q�[�W�̕�


};
