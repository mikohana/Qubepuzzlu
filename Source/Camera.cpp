#include "Camera.h"
#if true

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
#endif // false
#if  false

void Camera::SetLoolAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
	// クオータニオンをXMMATRIXに変換
	DirectX::XMMATRIX quaternionMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&this->));

	// クオータニオンによる回転を反映させる
	DirectX::XMVECTOR Eye = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&eye), quaternionMatrix);
	DirectX::XMVECTOR Focus = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&focus), quaternionMatrix);
	DirectX::XMVECTOR Up = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&up), quaternionMatrix);

	// 視点、注視点、上視点からビュー行列を作成
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	DirectX::XMStoreFloat4x4(&view, View);

	// ビューを逆行列化し、ワールド表列に戻す
	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, World);

	// カメラの方向を取り出す
	this->right.x = world._11;
	this->right.y = world._12;
	this->right.z = world._13;

	this->up.x = world._21;
	this->up.y = world._22;
	this->up.z = world._23;

	this->front.x = world._31;
	this->front.y = world._32;
	this->front.z = world._33;

	// 視点、注視点を保存
	this->eye = eye;
	this->focus = focus;
}

#endif //  false

//パースペクティブ設定
void Camera::SetPerspectiveFor(float fovY, float aspect, float nearZ, float farZ)
{
	//区角、画面比率、クリップ距離からプロジェクション行列を作成
	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);//LHは左手座標という意味、LはLeft,HはHand
	DirectX::XMStoreFloat4x4(&projection, Projection);
}

//
////クオータニオン更新処理
//void Camera::UpdateQuaternion(const DirectX::XMFLOAT3& axis, float angle)
//{
//	// 既存のクオータニオンを保持
//	DirectX::XMFLOAT4 oldQuaternion = this->quaternion;
//
//	// 角度をラジアンに変換
//	float radius = DirectX::XMConvertToRadians(angle);
//
//	// クオータニオンを生成
//	DirectX::XMFLOAT4 newQuaternion;
//	newQuaternion.w = cos(radius / 2);
//	newQuaternion.x = axis.x * sin(radius / 2);
//	newQuaternion.y = axis.y * sin(radius / 2);
//	newQuaternion.z = axis.z * sin(radius / 2);
//
//	// oldQuaternionとnewQuaternionがDirectX::XMFLOAT4型である場合の処理
//	DirectX::XMVECTOR oldQuaternionVector = DirectX::XMLoadFloat4(&oldQuaternion);
//	DirectX::XMVECTOR newQuaternionVector = DirectX::XMLoadFloat4(&newQuaternion);
//
//	DirectX::XMVECTOR resultQuaternionVector = DirectX::XMQuaternionMultiply(oldQuaternionVector, newQuaternionVector);
//	// 結果をDirectX::XMFLOAT4に変換
//	DirectX::XMFLOAT4 resultQuaternion;
//	DirectX::XMStoreFloat4(&resultQuaternion, resultQuaternionVector);
//
//	// 新しいクオータニオンを保存
//	this->quaternion = resultQuaternion;
//
//	// クオータニオンをXMMATRIXに変換
//	DirectX::XMMATRIX quaternionMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&this->quaternion));
//
//	// クオータニオンによる回転を反映させる
//	DirectX::XMStoreFloat3(&this->eye, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&this->eye), quaternionMatrix));
//	DirectX::XMStoreFloat3(&this->focus, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&this->focus), quaternionMatrix));
//}