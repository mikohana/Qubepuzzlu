#include "StageMain.h"
#include <imgui.h>
#include "Input/Input.h"


//コンストラクタ
StageMain::StageMain()
{
	//ステージモデルを読み込み
	model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
}
	StageMain::~StageMain()
	{
		//ステージモデルを破棄
		delete model;
	}

	//更新処理
	void StageMain::Update(float elapsedTime)
	{
		//WASDキーで回転
		UpdateStageRotate(elapsedTime);

		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

		DirectX::XMStoreFloat4x4(&stage_transform, R);
		//ステージ行列の更新処理
		model->UpdateTransform(stage_transform);
	}

	//描画処理
	void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
	{
		//シェーダーにモデル描画をしてもらう
		shader->Draw(dc, model);
		model->UpdateTransform(stage_transform);
	}

	


	//レイキャスト
	bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
	{
		return Collision::IntersecRayVsModel(start, end, model, hit);
	}


	//ステージの回転更新処理
	void StageMain::UpdateStageRotate(float elapsedTime)
	{
		//W
		GamePad& gamepad = Input::Instance().GetGamePad();
		
		if (gamepad.GetButton() & GamePad::KEY_W) // 上回転
		{
			isRotating = true;
			rotation.x -= rotateSpeed * elapsedTime;
		}
		//A
		if (gamepad.GetButton() & GamePad::KEY_A) // 左回転
		{
			isRotating = true;
			rotation.z -= rotateSpeed * elapsedTime;
		}
		//S
		if (gamepad.GetButton() & GamePad::KEY_S) // 下回転
		{
			isRotating = true;
			rotation.x += rotateSpeed * elapsedTime;
		}
		//D
		if (gamepad.GetButton() & GamePad::KEY_D) // 右回転
		{
			isRotating = true;
			rotation.z += rotateSpeed * elapsedTime;
		}
	}
