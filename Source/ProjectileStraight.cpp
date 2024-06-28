#include "ProjectileStraight.h"
#include "StageManager.h"

//コンストラクタ
ProjectileStraight::ProjectileStraight(ProjectileManager* manager) : Projectile(manager)
{
	//model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
	model = new Model("Data/Model/Sword/Sword.mdl");

	//表示サイズを更新
	 scale.x = scale.y = scale.z = 3.0f;
}

ProjectileStraight::~ProjectileStraight()
{
	delete model;
}

void ProjectileStraight::Update(float elapsedTime)
{
	//寿命処理
	 lifetime-= elapsedTime;
	if (lifetime <= 0)
	{
		Destroy();
	}
	//移動
	float speed = this->speed * elapsedTime;
	float mx = direction.x * speed;
	float mz = direction.z * speed;

	//オブジェクト行列更新
	UpdateTranceform();

	//モデル行列更新
	model->UpdateTransform(tranceform);

		//レイの開始始点と終点
		DirectX::XMFLOAT3 start = { position.x, position.y + 1.0f, position.z };
		DirectX::XMFLOAT3 end = { position.x + mx, position.y + 1.0f, position.z + mz };

		//レイキャストによる壁判定
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Ray = DirectX::XMVectorSubtract(End,Start);

			//壁の法線(P)
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			//入射ベクトルを法線に射影(V)
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Ray,Normal);

			//法線ベクトルと進行方向のドット積を計算して、反射幅として使用
			float correction = DirectX::XMVectorGetX(Dot);

			//反対ベクトル(O)の計算
			DirectX::XMVECTOR ReflectPos = DirectX::XMVectorSubtract(Ray, DirectX::XMVectorScale(Normal, 2.0f * correction));
			
			// Reflectdir = ReflectPos - hit.position
			DirectX::XMVECTOR HitPostion = DirectX::XMLoadFloat3(&hit.position);
			DirectX::XMVECTOR Reflectdir = DirectX::XMVectorSubtract(ReflectPos, HitPostion);


			// 交点からの反射の位置(R)を計算
			DirectX::XMStoreFloat3(&direction, ReflectPos);
			
		}
		else
		{
			position.x += mx;
			position.z += mz;
		}
		
	}
	




void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
}
