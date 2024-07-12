#pragma once
#include "Graphics/Graphics.h"
#include "Character.h"
//�{�b�N�X
class  Box: public Character
{
public:
	Box() {}
	~Box() override {}

	//�X�V����
	virtual void Update(float elapsedTime) = 0;

	//�`�揈��
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//�j��
	void Destroy();

	//�f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();


};