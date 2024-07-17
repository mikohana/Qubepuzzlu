#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "StageManager.h"
#include "StageMain.h"
#include "StageMoveFloor.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "BoxManager.h"
#include "Boxes.h"
#include "Goal.h"

#include "EffectManager.h"
#include "Input/Input.h"



//using namespace DirectX; 
//これを書くとDirectX::が不必要
// D3D11_VIEWPORT型のデータをViewport型に変換する関数
Viewport ConvertD3D11ViewportToViewport(const D3D11_VIEWPORT& d3dViewport)
{
	return Viewport{
		d3dViewport.TopLeftY,// Y軸の最上端
		d3dViewport.TopLeftX,// X軸の左端
		d3dViewport.Width,   // 幅
		d3dViewport.Height,  // 高さ
		d3dViewport.MinDepth,// 最小深度
		d3dViewport.MaxDepth // 最大深度
	};
}


// 初期化
void SceneGame::Initialize()
{

	//ステージ初期S化
	StageManager&  stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);


	//プレイヤー初期化
	player = new Player();

	//スカイボックスの読み込み
	skybox = new SkyBox("Data/SkyBox/Sky.png");

	//ゲージスプライト初期化
	gauge = new Sprite();

	//カメラコントローラー初期化
	cameraController = new CameraController();

	//ゴール初期化
	goal = new Goal();

	////エネミー初期化
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < 3; ++i)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i * 1.0f, 0, 5));
		slime->SetTerritory(slime->GetPosition(), 10.0f);
		enemyManager.Register(slime);
	}

	//ボックス初期化
	BoxManager& boxManager = BoxManager::Instance();

	// 10つのボックスの初期位置を設定する配列
	DirectX::XMFLOAT3 initialPositions[10] = {
		DirectX::XMFLOAT3(3.0f, 0.0f, -3.0f), // B
		DirectX::XMFLOAT3(-5.0f, 0.0f, 1.0f),// B
		DirectX::XMFLOAT3(3.0f, 0.0f, 3.0f), // B
		DirectX::XMFLOAT3(5.0f, 0.0f, 5.0f), // B

		DirectX::XMFLOAT3(-1.0f, 0.0f, -3.0f), // G
		DirectX::XMFLOAT3(1.0f, 0.0f, -5.0f), // G
		DirectX::XMFLOAT3(5.0f, 0.0f, -1.0f), // G
		DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f), // G

		DirectX::XMFLOAT3(-1.0f, 0.0f, -1.0f), // R
		DirectX::XMFLOAT3(-1.0f, 0.0f, 3.0f), // R
	};
	// 10つのボックスを作成
	for (int i = 0; i < 10; ++i)
	{
		Boxes* boxes = new Boxes(static_cast<BoxColor>(i / 4)); // 色を設定（0-1:赤、2-5:緑、6-9:青）

		// ボックスの初期位置を設定
		boxes->SetPosition(initialPositions[i]);

		// ボックスを BoxManager に登録
		boxManager.Register(boxes);
	}

	{
		// プレイヤーのボックスの初期位置を設定する配列
		DirectX::XMFLOAT3 playerBoxInitialPosition = DirectX::XMFLOAT3(-5.0f, 0.0f, 3.0f); // プレイヤーのボックス

		// プレイヤーのボックスを作成
		Boxes* playerBox = new Boxes(static_cast<BoxColor>(BoxColor::PLAYER)); // 種類を設定

		// プレイヤーのボックスの初期位置を設定
		playerBox->SetPosition(playerBoxInitialPosition);

		// プレイヤーのボックスを BoxManager に登録
		boxManager.Register(playerBox); 
	}

	{
		//ゴールの初期位置を設定
		DirectX::XMFLOAT3 goalInitPos = DirectX::XMFLOAT3(5.0f, 0.0f, -3.0f);//ゴールの初期位置変数

		//ゴールを作成
		Boxes* goal = new Boxes(static_cast<BoxColor>(BoxColor::GOAL));//種類を設定

		//プレイヤーのボックスの初期位置を設定
		goal->SetPosition(goalInitPos);

		//ゴールをBoxManagerに登録
		boxManager.Register(goal);

	}

	

	
	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLoolAt(
		DirectX::XMFLOAT3(0, 10, 0),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(1, 0, 0)
		);
	camera.SetPerspectiveFor(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	
}

// 終了化
void SceneGame::Finalize()
{
	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//ゲージスプライト終了化
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

	//プレイヤー終了化
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//スカイボックス終了化
	if (skybox != nullptr)
	{
		delete skybox;
		skybox = nullptr;
	}

	//ゴール終了化
	if (goal != nullptr)
	{
		delete goal;
		goal = nullptr;
	}

	//エネミー終了化
	EnemyManager::Instance().Clear();

	//ボックス終了化
	BoxManager::Instance().Clear();
	
	//ステージ終了化
	StageManager::Instance().Clear();

			
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	//カメラコントローラー更新処理
	CameraController::Instance().Update(elapsedTime);

	//ステージ更新処理
	StageManager::Instance().Update(elapsedTime);

	//プレイヤー更新処理
	player->Update(elapsedTime);

	//ボックス更新処理
	BoxManager::Instance().Update(elapsedTime);

	//ゴール更新処理
	goal->Update(elapsedTime);

	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//ステージ描画
		StageManager::Instance().Render(dc, shader);
		//プレイヤー描画
		player->Render(dc, shader);
		//ゴール描画
		//goal->Render(dc, shader);
		//エネミー描画
		EnemyManager::Instance().Render(dc, shader);
		//ボックス描画
		BoxManager::Instance().Render(dc, shader);


		shader->End(dc);
	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{
		skybox->Render(dc, rc);
		RenderEnemyGauge(dc, rc.view, rc.projection);
		
	}

	// 2DデバッグGUI描画
	{
		//プレイヤーデバッグ描画
		player->DrawDebugGuI();

		cameraController->DrawDebugGuI();
		
	}

	
}


//エネミーHPゲージ描画
void SceneGame::RenderEnemyGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	
	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports,&viewport);
	
	// D3D11_VIEWPORTをViewport型に変換
	Viewport convertedViewport = ConvertD3D11ViewportToViewport(viewport);
	convertedViewport.MinZ = 0.0f;
	convertedViewport.MaxZ = 1.0f;

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projectile = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	//全ての敵の頭上にHPゲージを表示
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	
	for (int i = 0; i < enemyCount; i++)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//エネミーの現在の頭上の位置を計算
		DirectX::XMFLOAT3 position = enemy->GetPosition();
		float height = enemy->GetHeight();

		// 現在の健康状態を最大健康状態で割り、パーセンテージに変換
		float gaugeW = static_cast<float>(enemy->GetHealth()) / enemy->GetMaxHealth() * 100.0f;

		//頭上の位置の位置を計算
		DirectX::XMFLOAT3 headPosition = position;

		headPosition.y = height;

		//ワールド座標からスクリーン座標に変換
		DirectX::XMFLOAT2 screenPos = WorldToScreen(
			headPosition,
			convertedViewport,
			Projectile,
			View,
			worldMatrix
		);


		//敵の頭上にHPゲージのスプライトを表示する
		gauge->Render(dc,                      //レンダリングコンテキスト
			screenPos.x, screenPos.y,           //スクリーン座標座標系でのスプライト描画
			gaugeW, 10.0f,                         //スプライトの幅高さ
			0, 0, 0, 0,                              //テクスチャスプライトの位置とサイズ
			0,
			1, 0, 0, 1);
	}
#if false
	
	//エネミー配置処理
		Mouse& mouse = Input::Instance().GetMouse();
		if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			//カーソル座標を取得
			DirectX::XMFLOAT3 screenPosition;
			screenPosition.x = static_cast<float>(mouse.GetPositionX());
			screenPosition.y = static_cast<float>(mouse.GetPositionY());

			//ビューポートの値を設定
			float minDepth = 0.0f;
			float maxDepth = 1.0f;
			
			//スクリーン座標のZ軸のビューポートの最小深度値から最大深度値までの範囲内に設定
			//レイの開始点と始点
			DirectX::XMFLOAT3 rayStart = ScreenToWorld(
				screenPosition.x, screenPosition.y,minDepth,
				convertedViewport,
				Projectile,
				View,
				worldMatrix);

			//レイの終点
			DirectX::XMFLOAT3 rayEnd = ScreenToWorld(
				screenPosition.x, screenPosition.y, maxDepth,
				convertedViewport,
				Projectile,
				View,
				worldMatrix);
			
			
			//レイとモデル(ステージ)の交差判定を行う
			HitResult hitResult;
			
			//交差があった場合、エネミーを配置
			if (StageManager::Instance().RayCast(rayStart,rayEnd,hitResult))
			{
				//新しいエネミーインスタンスを作成
				Enemy* newEnemy = new EnemySlime();
				//エネミーの初期位置を設定
				newEnemy->SetPosition(hitResult.position);

				//EnemyManagerに新しく追加されたエネミーを登録
				enemyManager.Register(newEnemy);
			}
		}
#endif // false

	}

