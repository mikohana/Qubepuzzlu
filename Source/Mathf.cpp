#include "Mathf.h"
#include <cstdlib>

//���`�⊮
float Mathf::Lerp(float a, float b, float t)
{
	return a * (1.0f - t) + (b * t);
}

//�w��͈͂̃����_���l���v�Z����
float Mathf::RandomRange(float min, float max)
{
	//�����_���Ȓl�𐶐�����
	float range = max - min;
	float randomValue = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

	//�w��͈͓��̒l�ɕϊ�
	float result = randomValue * (max - min) + min;
	return result;
}

