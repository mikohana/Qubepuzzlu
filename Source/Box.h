#pragma once
#include "Graphics/Graphics.h"
#include "Character.h"
//ボックス
class  Box: public Character
{
public:
	Box() {}
	~Box() override {}

	//更新処理
	virtual void Update(float elapsedTime) = 0;

	//描画処理
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//破棄
	void Destroy();

	//デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();


};