#pragma once

#include "Graphics/Model.h"
#include "Box.h"

enum  class BoxColor
{
	BLUE,
	GREEN,
	RED,
	PLAYER,
	GOAL,
	NONE
};

//ボックス
class  Boxes : public Box
{
public:
	Boxes(BoxColor color);
	~Boxes() override;

	void Update(float elapsedTime) override;

	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	BoxColor GetColor() const { return color; }

	//デバッグプリミティブ
	void DrawDebugPrimitive() override;

private:
	//Model* model = nullptr;
	std::vector<Model*> models[5];
	BoxColor color;

private:
	float territoryRange = 10.0f;
	float moveSpeed = 3.0f;
	float turnSpeed = DirectX::XMConvertToRadians(360);
	float stateTimer = 0.0f;
	float searchRange = 5.0f;
	float attackRange = 1.5f;

};

