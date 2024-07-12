#include "Box.h"
#include "BoxManager.h"
#include "Graphics/Graphics.h"

void Box::Destroy()
{
	// BoxManager�̃C���X�^���X���擾
	BoxManager& boxManager = BoxManager::Instance();
	// EnemyManager���炱��Enemy���폜
	boxManager.Remove(this);
}

void Box::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//�Փ˔���p�̃f�o�b�O����`��
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

	//�Փ˔���p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 1, 1, 0));


}
