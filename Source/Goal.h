#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectilrManager.h"
#include "Effect.h"

//�S�[��
class Goal : public Character
{
public:
	Goal();
	~Goal() override;

	//�C���X�^���X�擾
	static Goal& Instance();

	//�X�V����
	void Update(float elapedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//�f�o�b�O�pGUI�`�揈��
	//void DrawDebugGuI();

	//�����蔻��
	//void DrawDebugPrimitive();

	//�v���C���[�ƃS�[���Ƃ̏Փ˔���
	void CollisionPlayerVsGoal();

	//�e�ۓ��͏���
	void InputProjectile();



private:
	Model* model = nullptr;

	ProjectileManager projectileManager;

	Effect* hitEffect = nullptr;


};