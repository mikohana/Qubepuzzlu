#pragma once

#include "Graphics/Sprite.h"
#include"Scene.h"
#include <thread>
#include <atomic>


//ローディングシーン
class  SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene) :  nextScene(nextScene){}
	~SceneLoading() override {}

	//初期化
	void Initialize() override;

	//終了化
	void Finalize() override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render() override;


private:
	//ローディングスレッド
	static void LoadingThread(SceneLoading* scene);

private:
	float angle = 0.0f;
	Sprite* sprite = nullptr;
	Scene* nextScene = nullptr;
	std::thread* thread = nullptr;

	
	
	

};

 