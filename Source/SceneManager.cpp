#include "SceneManager.h"

//更新処理
void SceneManager::Update(float elapsedTime)
{
    // 新しいシーンが設定されている場合
    if (nextScene != nullptr)
    {
        // 古いシーンの終了処理
        Clear();

        // 新しいシーンを設定
        currentScene = nextScene;
        nextScene = nullptr;

       //シーン初期化処理
        if (!currentScene->IsReady())
        {
            currentScene->Initialize();
        }
    }

    // 現在のシーンが存在する場合、更新処理を行う
    if (currentScene != nullptr)
    {
        currentScene->Update(elapsedTime);
    }
}

//描画処理
void SceneManager::Render()
{
	if (currentScene != nullptr)
	{
		currentScene->Render();
	}
}

//シーンクリア
void SceneManager::Clear()
{
	if (currentScene != nullptr)
	{
		currentScene->Finalize();
		delete currentScene;
		currentScene = nullptr;
	}
}

//シーン切り替え
void SceneManager::ChangeScene(Scene* scene)
{   
    
	// 新しいシーンを設定
	nextScene = scene;
}

