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
//�����������DirectX::���s�K�v
// D3D11_VIEWPORT�^�̃f�[�^��Viewport�^�ɕϊ�����֐�
Viewport ConvertD3D11ViewportToViewport(const D3D11_VIEWPORT& d3dViewport)
{
	return Viewport{
		d3dViewport.TopLeftY,// Y���̍ŏ�[
		d3dViewport.TopLeftX,// X���̍��[
		d3dViewport.Width,   // ��
		d3dViewport.Height,  // ����
		d3dViewport.MinDepth,// �ŏ��[�x
		d3dViewport.MaxDepth // �ő�[�x
	};
}


// ������
void SceneGame::Initialize()
{

	//�X�e�[�W����S��
	StageManager&  stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);


	//�v���C���[������
	player = new Player();

	//�X�J�C�{�b�N�X�̓ǂݍ���
	skybox = new SkyBox("Data/SkyBox/Sky.png");

	//�Q�[�W�X�v���C�g������
	gauge = new Sprite();

	//�J�����R���g���[���[������
	cameraController = new CameraController();

	//�S�[��������
	goal = new Goal();

	////�G�l�~�[������
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < 3; ++i)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i * 1.0f, 0, 5));
		slime->SetTerritory(slime->GetPosition(), 10.0f);
		enemyManager.Register(slime);
	}

	//�{�b�N�X������
	BoxManager& boxManager = BoxManager::Instance();

	// 10�̃{�b�N�X�̏����ʒu��ݒ肷��z��
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
	// 10�̃{�b�N�X���쐬
	for (int i = 0; i < 10; ++i)
	{
		Boxes* boxes = new Boxes(static_cast<BoxColor>(i / 4)); // �F��ݒ�i0-1:�ԁA2-5:�΁A6-9:�j

		// �{�b�N�X�̏����ʒu��ݒ�
		boxes->SetPosition(initialPositions[i]);

		// �{�b�N�X�� BoxManager �ɓo�^
		boxManager.Register(boxes);
	}

	{
		// �v���C���[�̃{�b�N�X�̏����ʒu��ݒ肷��z��
		DirectX::XMFLOAT3 playerBoxInitialPosition = DirectX::XMFLOAT3(-5.0f, 0.0f, 3.0f); // �v���C���[�̃{�b�N�X

		// �v���C���[�̃{�b�N�X���쐬
		Boxes* playerBox = new Boxes(static_cast<BoxColor>(BoxColor::PLAYER)); // ��ނ�ݒ�

		// �v���C���[�̃{�b�N�X�̏����ʒu��ݒ�
		playerBox->SetPosition(playerBoxInitialPosition);

		// �v���C���[�̃{�b�N�X�� BoxManager �ɓo�^
		boxManager.Register(playerBox); 
	}

	{
		//�S�[���̏����ʒu��ݒ�
		DirectX::XMFLOAT3 goalInitPos = DirectX::XMFLOAT3(5.0f, 0.0f, -3.0f);//�S�[���̏����ʒu�ϐ�

		//�S�[�����쐬
		Boxes* goal = new Boxes(static_cast<BoxColor>(BoxColor::GOAL));//��ނ�ݒ�

		//�v���C���[�̃{�b�N�X�̏����ʒu��ݒ�
		goal->SetPosition(goalInitPos);

		//�S�[����BoxManager�ɓo�^
		boxManager.Register(goal);

	}

	

	
	//�J���������ݒ�
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

// �I����
void SceneGame::Finalize()
{
	//�J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//�Q�[�W�X�v���C�g�I����
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

	//�v���C���[�I����
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//�X�J�C�{�b�N�X�I����
	if (skybox != nullptr)
	{
		delete skybox;
		skybox = nullptr;
	}

	//�S�[���I����
	if (goal != nullptr)
	{
		delete goal;
		goal = nullptr;
	}

	//�G�l�~�[�I����
	EnemyManager::Instance().Clear();

	//�{�b�N�X�I����
	BoxManager::Instance().Clear();
	
	//�X�e�[�W�I����
	StageManager::Instance().Clear();

			
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	//�J�����R���g���[���[�X�V����
	CameraController::Instance().Update(elapsedTime);

	//�X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);

	//�v���C���[�X�V����
	player->Update(elapsedTime);

	//�{�b�N�X�X�V����
	BoxManager::Instance().Update(elapsedTime);

	//�S�[���X�V����
	goal->Update(elapsedTime);

	//�G�l�~�[�X�V����
	EnemyManager::Instance().Update(elapsedTime);

	
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

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
		//�X�e�[�W�`��
		StageManager::Instance().Render(dc, shader);
		//�v���C���[�`��
		player->Render(dc, shader);
		//�S�[���`��
		//goal->Render(dc, shader);
		//�G�l�~�[�`��
		EnemyManager::Instance().Render(dc, shader);
		//�{�b�N�X�`��
		BoxManager::Instance().Render(dc, shader);


		shader->End(dc);
	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{
		skybox->Render(dc, rc);
		RenderEnemyGauge(dc, rc.view, rc.projection);
		
	}

	// 2D�f�o�b�OGUI�`��
	{
		//�v���C���[�f�o�b�O�`��
		player->DrawDebugGuI();

		cameraController->DrawDebugGuI();
		
	}

	
}


//�G�l�~�[HP�Q�[�W�`��
void SceneGame::RenderEnemyGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	
	//�r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports,&viewport);
	
	// D3D11_VIEWPORT��Viewport�^�ɕϊ�
	Viewport convertedViewport = ConvertD3D11ViewportToViewport(viewport);
	convertedViewport.MinZ = 0.0f;
	convertedViewport.MaxZ = 1.0f;

	//�ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projectile = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	//�S�Ă̓G�̓����HP�Q�[�W��\��
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	
	for (int i = 0; i < enemyCount; i++)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//�G�l�~�[�̌��݂̓���̈ʒu���v�Z
		DirectX::XMFLOAT3 position = enemy->GetPosition();
		float height = enemy->GetHeight();

		// ���݂̌��N��Ԃ��ő匒�N��ԂŊ���A�p�[�Z���e�[�W�ɕϊ�
		float gaugeW = static_cast<float>(enemy->GetHealth()) / enemy->GetMaxHealth() * 100.0f;

		//����̈ʒu�̈ʒu���v�Z
		DirectX::XMFLOAT3 headPosition = position;

		headPosition.y = height;

		//���[���h���W����X�N���[�����W�ɕϊ�
		DirectX::XMFLOAT2 screenPos = WorldToScreen(
			headPosition,
			convertedViewport,
			Projectile,
			View,
			worldMatrix
		);


		//�G�̓����HP�Q�[�W�̃X�v���C�g��\������
		gauge->Render(dc,                      //�����_�����O�R���e�L�X�g
			screenPos.x, screenPos.y,           //�X�N���[�����W���W�n�ł̃X�v���C�g�`��
			gaugeW, 10.0f,                         //�X�v���C�g�̕�����
			0, 0, 0, 0,                              //�e�N�X�`���X�v���C�g�̈ʒu�ƃT�C�Y
			0,
			1, 0, 0, 1);
	}
#if false
	
	//�G�l�~�[�z�u����
		Mouse& mouse = Input::Instance().GetMouse();
		if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			//�J�[�\�����W���擾
			DirectX::XMFLOAT3 screenPosition;
			screenPosition.x = static_cast<float>(mouse.GetPositionX());
			screenPosition.y = static_cast<float>(mouse.GetPositionY());

			//�r���[�|�[�g�̒l��ݒ�
			float minDepth = 0.0f;
			float maxDepth = 1.0f;
			
			//�X�N���[�����W��Z���̃r���[�|�[�g�̍ŏ��[�x�l����ő�[�x�l�܂ł͈͓̔��ɐݒ�
			//���C�̊J�n�_�Ǝn�_
			DirectX::XMFLOAT3 rayStart = ScreenToWorld(
				screenPosition.x, screenPosition.y,minDepth,
				convertedViewport,
				Projectile,
				View,
				worldMatrix);

			//���C�̏I�_
			DirectX::XMFLOAT3 rayEnd = ScreenToWorld(
				screenPosition.x, screenPosition.y, maxDepth,
				convertedViewport,
				Projectile,
				View,
				worldMatrix);
			
			
			//���C�ƃ��f��(�X�e�[�W)�̌���������s��
			HitResult hitResult;
			
			//�������������ꍇ�A�G�l�~�[��z�u
			if (StageManager::Instance().RayCast(rayStart,rayEnd,hitResult))
			{
				//�V�����G�l�~�[�C���X�^���X���쐬
				Enemy* newEnemy = new EnemySlime();
				//�G�l�~�[�̏����ʒu��ݒ�
				newEnemy->SetPosition(hitResult.position);

				//EnemyManager�ɐV�����ǉ����ꂽ�G�l�~�[��o�^
				enemyManager.Register(newEnemy);
			}
		}
#endif // false

	}

