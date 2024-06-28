#pragma once
#include "Graphics/Graphics.h"
#include "Projectile.h"

//’¼i’eŠÛ
class ProjectileStraight : public Projectile
{
public:
	//ProjectileStraight();
	ProjectileStraight(ProjectileManager* manager);
	~ProjectileStraight() override;

	//XVˆ—
	void Update(float elapsedTime) override;

	//•`‰æˆ—
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	//”­Ëˆ—
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& postion);

private:
	Model* model = nullptr;

	float lifetime = 3.0f;
	float speed = 10.0f;
};