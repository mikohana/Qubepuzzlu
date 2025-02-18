#pragma once

#include "Graphics/Graphics.h"
//相互インクルードされないように前方宣言する
class ProjectileManager;

//弾丸
class Projectile
{
public:
	//Projectile() {}
	Projectile(ProjectileManager* manager);
	virtual ~Projectile() {}

		//更新処理
		virtual void Update(float elaspedTime) = 0;

		//描画処理
		virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

		//破棄
		void Destroy();

		//デバッグプリミティブ描画
		virtual void DebugPrimitive();

		//位置取得
		const DirectX::XMFLOAT3& GetPosition() const { return position; };

		//方向取得
		const DirectX::XMFLOAT3& GetDirection() const { return direction; };

		//スケール取得
		const DirectX::XMFLOAT3& GetScale() const { return scale; };

		//半径取得
		float GetRadius() const { return radius; }
	
protected:
	//行列更新処理
	void UpdateTranceform();

	ProjectileManager* manager = nullptr;

protected:
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 direction = { 0,0,1 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT3 velocity = { 0,0,0 };
	DirectX::XMFLOAT4X4 tranceform = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1};
	float radius = 0.5f;
};