#include "Projectile.h"
#include "ProjectilrManager.h"
//デバッグプリミティブ描画


Projectile::Projectile(ProjectileManager* manager) : manager(manager)
{
	//生成時にマネージャーを生成する
	manager->Register(this);
}

void Projectile::Destroy()
{
	//マネージャーから自身を削除する
	manager->Remove(this);
}

void Projectile::DebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	
}

//行列更新処理
void Projectile::UpdateTranceform()
{
	DirectX::XMVECTOR Up, Right, Front;
	// 前ベクトルを算出
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);

	//仮の上ベクトルを用意
	Up = DirectX::XMVectorSet(0.001f, 1, 0, 0);
	Up = DirectX::XMVector3Normalize(Up);

	// 右ベクトルを算出
	Right = DirectX::XMVector3Cross(Up, Front);
	Right = DirectX::XMVector3Normalize(Right);

	// 上ベクトルを算出
	// RightとFrontの外積を取ることで上ベクトルを計算
	Up = DirectX::XMVector3Cross(Front,Right);

	// 計算式を取り出し
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	//算出した軸からベクトル行列を作成
	tranceform._11 = right.x * scale.x;
	tranceform._12 = right.y * scale.x;
	tranceform._13 = right.z * scale.x;
	tranceform._14 =0.0f;
	tranceform._21 = up.x * scale.x;
	tranceform._22 = up.y * scale.y;
	tranceform._23 = up.z * scale.z;
	tranceform._24 =0.0f;
	tranceform._31 = front.x * scale.z;
	tranceform._32 = front.y * scale.z;
	tranceform._33 = front.z * scale.z;
	tranceform._34 =0.0f;
	tranceform._41 = position.x;
	tranceform._42 = position.y;
	tranceform._43 = position.z;
	tranceform._44 =1.0f;

	//発射方向
	this->direction = front;
	
}

