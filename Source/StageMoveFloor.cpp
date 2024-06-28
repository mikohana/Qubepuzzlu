#include "StageMoveFloor.h"

//コンストラクタ
StageMoveFloor::StageMoveFloor()
{
	scale.x = scale.z = 3.0f;
	scale.y = 0.5f;

	//ステージモデルを読み込み
	model = new Model("Data/Model/Cube/Cube.mdl");
}

StageMoveFloor::~StageMoveFloor()
{
	//ステージモデルを破棄
	delete model;
}

//更新処理
void StageMoveFloor::Update(float elapsedTime)
{
	//前回の情報を保存 
	oldTransform = transform;
	oldAngle = angle;

	//ステージからゴールまでの距離を算出する
	DirectX::XMVECTOR Start  =DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR Goal   =DirectX::XMLoadFloat3(&goal);
	DirectX::XMVECTOR Vec    =DirectX::XMVectorSubtract(Goal, Start);
	DirectX::XMVECTOR Length =DirectX::XMVector3Length(Vec);
	float lenght;
	DirectX::XMStoreFloat(&lenght, Length);

	//スタートからゴールまでの間を一秒で進む割合(0.0～1.0)を算出する
	float speed = moveSpeed * elapsedTime;
	float speedRate = speed / lenght;
	moveRate += speedRate;
	//ゴールに到達、またはスタートに戻った場合、移動方向を反転させる
	if (moveRate <= 0.0f || moveRate >= 1.0f)
	{
		moveSpeed = -moveSpeed;
	}
	//線形保管で位置を算出する
	DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
	DirectX::XMStoreFloat3(&position, Position);

	//回転
	angle.x += torque.x * elapsedTime;
	angle.y += torque.y * elapsedTime;
	angle.z += torque.z * elapsedTime;

	//行列更新
	UpdateTransform();

	////モデル行列更新
	//model->UpdateTransform(transform);

	//レイキャスト用ににモデル空間表列にするために単位行列を渡す
	const DirectX::XMFLOAT4X4 transformIdentity = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	model->UpdateTransform(transformIdentity);
}

//描画処理
void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	//表示用のためワールド行列に更新する
	model->UpdateTransform(transform);
	shader->Draw(dc, model);
}

//レイキャスト
bool StageMoveFloor::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	//前回のワールド行列と逆行列化する
	DirectX::XMMATRIX WorldTransform = DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&oldTransform));

	//前回のローカル空間でレイに変換
	DirectX::XMVECTOR RayStart = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR RayEnd = DirectX::XMLoadFloat3(&end);
	RayStart = DirectX::XMVector3TransformCoord(RayStart, WorldTransform);
	RayEnd = DirectX::XMVector3TransformCoord(RayEnd, WorldTransform);
	//ローカル空間でのレイとの交点を求める
	DirectX::XMFLOAT3 localStart, localEnd;
	DirectX::XMStoreFloat3(&localStart, RayStart);
	DirectX::XMStoreFloat3(&localEnd, RayEnd);

	HitResult localHit;
	if (Collision::IntersecRayVsModel(localStart, localEnd, model, localHit))
	{
		//前回のローカル空間から今回のワールド空間へ変換
		//DirectX::XMStoreFloat4x4(&transform, oldInverseTransform);
		
		
		DirectX::XMVECTOR hitPosint = DirectX::XMLoadFloat3(&localHit.position);
		hitPosint = DirectX::XMVector3TransformCoord(hitPosint, DirectX::XMLoadFloat4x4(&transform));
		
		//変換された交点を格納
		DirectX::XMStoreFloat3(&hit.position, hitPosint);
		hit.rotation.x =- oldAngle.x + angle.x;
		hit.rotation.y =- oldAngle.y + angle.y;
		hit.rotation.z =- oldAngle.z + angle.z;

		//衝突情報を設定
		hit.normal = localHit.normal;
		hit.distance = localHit.distance;

	
		return true;
	}
	return false;
}

//行列更新処理
void StageMoveFloor::UpdateTransform()
{
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, W);
}
