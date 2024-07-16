#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Input/Input.h"
#include <Camera.h>

//������
void SceneTitle::Initialize()
{
	//�X�v���C�g������
	sprite = new Sprite("Data/Sprite/Title.png");

	TitleCube = new Cube("Data/Model/Cube/Cube.mdl");

	//�J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLoolAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFor(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);
}

//�I����
void SceneTitle::Finalize()
{
	//�X�v���C�g
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
	if (TitleCube != nullptr)
	{
		delete TitleCube;
		TitleCube = nullptr;
	}
}

float angle = 0.0f;

//�X�V����
void SceneTitle::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//�����{�^������������Q�[���V�[���ɐ؂�ւ�
	const GamePadButton anyButton =
		GamePad::BTN_A
		| GamePad::BTN_B
		| GamePad::BTN_X
		| GamePad::BTN_Y
		;
	if (gamePad.GetButton())
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}

	angle += 0.1f;

	TitleCube->SetAngle({ angle , angle , 0.0f });

	TitleCube->UpdateTransform();
}

//2D�`�揈��
void SceneTitle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	//2D�X�v���C�g�`��
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screemHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(sprite->GetTextureWidth());
		float textureHeight = static_cast<float>(sprite->GetTextureHeight());
		//�^�C�g���X�v���C�g�`��
		sprite->Render(dc,
			screenWidth * 0.25f, screemHeight * 0.25f, screenWidth / 2, screemHeight / 2,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1);
	}
	
	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	//�J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);


		//shader->Draw(dc, TitleCube);

		TitleCube->Render(dc, shader);


		shader->End(dc);
	}
}