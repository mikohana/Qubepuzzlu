#include "StageManager.h"

//更新処理
void StageManager::Update(float elapsedTime)
{
	for (Stage* stage : stages)
	{
		stage->Update(elapsedTime);
	}
}

//描画処理
void StageManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (Stage* stage : stages)
	{
		stage->Render(context, shader);
	}
}

//ステージ登録
void StageManager::Register(Stage* stage)
{
	stages.emplace_back(stage);
}

//ステージ全削除
void StageManager::Clear()
{
	for (Stage* stage : stages)
	{
		delete stage;
	}
	stages.clear();
}

//レイキャスト
bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	bool result = false;
	for (Stage* stage : stages)
	{
		if (stage->RayCast(start, end, hit))
		{
			result = true; 
		}
	}
	return result;
}
