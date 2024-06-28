#include "Mathf.h"
#include <cstdlib>

//線形補完
float Mathf::Lerp(float a, float b, float t)
{
	return a * (1.0f - t) + (b * t);
}

//指定範囲のランダム値を計算する
float Mathf::RandomRange(float min, float max)
{
	//ランダムな値を生成する
	float range = max - min;
	float randomValue = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

	//指定範囲内の値に変換
	float result = randomValue * (max - min) + min;
	return result;
}

