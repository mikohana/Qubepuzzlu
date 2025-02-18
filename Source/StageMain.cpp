#include "StageMain.h"
#include <imgui.h>
#include "Input/Input.h"


//コンストラクタ
StageMain::StageMain()
{
	//ステージモデルを読み込み
	model = new Model("Data/Model/ExampleStage/stage1.mdl");
	/*stages[0].push_back(new Model("Data/Model/ExampleStage/stage0.mdl"));
	stages[1].push_back(new Model("Data/Model/ExampleStage/stage1.mdl"));
	stages[2].push_back(new Model("Data/Model/ExampleStage/stage2.mdl"));
	stages[3].push_back(new Model("Data/Model/ExampleStage/stage3.mdl"));
	stages[4].push_back(new Model("Data/Model/ExampleStage/stage4.mdl"));
	stages[5].push_back(new Model("Data/Model/ExampleStage/stage5.mdl"));*/
	scale = {0.02f,0.02f,0.02f};
	
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
		//UpdateStageRotate(elapsedTime);

		//回転行列の作成
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

		//スケーリング行列を作成
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

		//位置行列の作成
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

		//回転とスケールを掛け合わす
		DirectX::XMMATRIX M = R * S * T;

		DirectX::XMStoreFloat4x4(&stage_transform,M);
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
			rotation.x -= rotateSpeed * elapsedTime;
		}
		//A
		else if (gamepad.GetButton() & GamePad::KEY_A) // 左回転
		{
			rotation.z -= rotateSpeed * elapsedTime;
		}
		//S
		else if (gamepad.GetButton() & GamePad::KEY_S) // 下回転
		{	
			rotation.x += rotateSpeed * elapsedTime;
		}
		//D
		else if (gamepad.GetButton() & GamePad::KEY_D) // 右回転
		{
			rotation.z += rotateSpeed * elapsedTime;
		}

		else
		{
			rotation = {};
		}
	}
