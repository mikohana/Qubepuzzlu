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

	////���ׂẴ{�b�N�X�ƃS�[���̓����蔻��
	//void CollisionBoxesVsGoal();

private:
	Model* model = nullptr;

	ProjectileManager projectileManager;

	Effect* hitEffect = nullptr;


};