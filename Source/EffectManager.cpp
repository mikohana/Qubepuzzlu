#include "Graphics/Graphics.h"
#include "EffectManager.h"

void EffectManager::Initialize()
{
	Graphics& graphics = Graphics::Instance();

	auto device = graphics.GetDevice();

	auto deviceContext = graphics.GetDeviceContext();
	
	//Effekseerレンダラ生成
	effekseerRenderer = EffekseerRendererDX11::Renderer::Create(device, deviceContext, 2048);

	//Effekseerマネージャー生成
	effekseerManager = Effekseer::Manager::Create(2048);

	//Effekseerレンダラの各種設定(特別なカスタマイズをしない場合は定形的に以下の設定でok)
	effekseerManager->SetSpriteRenderer(effekseerRenderer->CreateSpriteRenderer());
	effekseerManager->SetRibbonRenderer(effekseerRenderer->CreateRibbonRenderer());
	effekseerManager->SetRingRenderer(effekseerRenderer->CreateRingRenderer());
	effekseerManager->SetTrackRenderer(effekseerRenderer->CreateTrackRenderer());
	effekseerManager->SetModelRenderer(effekseerRenderer->CreateModelRenderer());
	//Effekseer内でのローダーの設定(特別なカスタマイズをしない場合は以下の設定でおけ)
	effekseerManager->SetTextureLoader(effekseerRenderer->CreateTextureLoader());
	effekseerManager->SetModelLoader(effekseerRenderer->CreateModelLoader());
	effekseerManager->SetMaterialLoader(effekseerRenderer->CreateMaterialLoader());
	//Effkseerを左手座標系で計算する
	effekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

//終了化
void EffectManager::Finelize()
{
	//EffekseerManagerなどはスマートポインタによって破棄されるので何もしない
}

//更新処理
void EffectManager::Update(float elapsedTime)
{
	effekseerManager->Update(elapsedTime * 60.0f);
}

//描画処理
void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	//ビュー＆プロジェクション行列をEffekseerレンダラに設定
	effekseerRenderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
	effekseerRenderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

	//Effekseer描画開始
	effekseerRenderer->BeginRendering();

	//Effekseer描画関数
	//マネージャー単位で描画するので描画順を制御するばあはマネージャーを複数個作成し、
	//Draw()関数を実行する順序で制御出来そう
	effekseerManager->Draw();
	//Effekseer描画終了
	effekseerRenderer->EndRendering();
}



