#include "Box.h"
#include "BoxManager.h"
#include "Graphics/Graphics.h"

void Box::Destroy()
{
	// BoxManagerのインスタンスを取得
	BoxManager& boxManager = BoxManager::Instance();
	// EnemyManagerからこのEnemyを削除
	boxManager.Remove(this);
}

void Box::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//衝突判定用のデバッグ球を描画
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

	//衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 1, 1, 0));


}
