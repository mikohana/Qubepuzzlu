#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Cube.h"

//�^�C�g���V�[��
class SceneTitle : public Scene
{
public:
   SceneTitle() {}
   ~SceneTitle() override {}

   //������
   void Initialize() override;

   //�I����
   void Finalize() override;

   //�X�V����
   void Update(float elapsedTime) override;

   //�`�揈��
   void Render() override;

   void Render(ID3D11DeviceContext* dc, Shader* shader);

private:
	Sprite* sprite = nullptr;
	Cube* TitleCube = nullptr;
};