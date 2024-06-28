#include "Camera.h"

void Camera::SetLoolAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
	//視点、注視点、上視点からビュー行列を作成
	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);    //カメラの位置を表すベクトル
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);//カメラが注視している点を表すベクトル
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);      //カメラの上向きのベクトル
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	DirectX::XMStoreFloat4x4(&view, View);

	//ビューを逆行列化し、ワールド表列に戻す
	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, World);

	//カメラの方向を取り出す
	this->right.x = world._11;
	this->right.y = world._12;
	this->right.z = world._13;

	this->up.x = world._21;
	this->up.y = world._22;
	this->up.z = world._23;

	this->front.x = world._31;
	this->front.y = world._32;
	this->front.z = world._33;

	//視点、注視点を保存
	this->eye = eye;
	this->focus = focus;
}

//パースペクティブ設定
void Camera::SetPerspectiveFor(float fovY, float aspect, float nearZ, float farZ)
{
	//区角、画面比率、クリップ距離からプロジェクション行列を作成
	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);//LHは左手座標という意味、LはLeft,HはHand
	DirectX::XMStoreFloat4x4(&projection, Projection);
}
