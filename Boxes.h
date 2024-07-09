#pragma once

#include "Graphics/Model.h"
#include "Box.h"

//ボックス
class  Boxes : public Box
{
public:
	Boxes();
	~Boxes() override;

	void Update(float elapsedTime) override;

	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//デバッグプリミティブ
	void DrawDebugPrimitive() override;

private:
	Model* model = nullptr;

private:
	float territoryRange = 10.0f;
	float moveSpeed = 3.0f;
	float turnSpeed = DirectX::XMConvertToRadians(360);
	float stateTimer = 0.0f;
	float searchRange = 5.0f;
	float attackRange = 1.5f;

};

