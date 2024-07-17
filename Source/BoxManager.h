#pragma once
#include <vector>
#include <set>
#include "Box.h"
#include "Boxes.h"
#include "Goal.h"
//�{�b�N�X�}�l�[�W���[
class  BoxManager : public Character
{
private:
	BoxManager() {};
	~BoxManager() {};

public:
	//�C���X�^���X�̎擾
	static BoxManager& Instance()
	{
		static BoxManager instance;
		return instance;
	}


	void Update(float elapsedTime);

	void Render(ID3D11DeviceContext* dc, Shader* shader);
	//�{�b�N�X�o�^
	void Register(Box* box);

	void Clear();

	//�f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	//�{�b�N�X���擾
	int GetBoxCount() const { return static_cast<int>(box.size()); }

	//�{�b�N�X�擾
	Box* GetBox(int index) const { return box.at(index); }

	//����̎�ނ̃{�b�N�X���擾���郁�\�b�h
	Boxes* GetBoxByColor(BoxColor color)
	{
		//�o�^����Ă���S�Ẵ{�b�N�X���`
		for (Boxes* box : boxes)
		{
			//�{�b�N�X�̐F�̎w�肳�ꂽ�F�ƈ�v����ꍇ�A���̃{�b�N�X��Ԃ�
			if (box->GetColor() == color)
			{
				return box;
			}
		}
		//�w�肳�ꂽ�F�̃{�b�N�X���Ȃ�������nullptr
		return nullptr;
	}

	//���ׂẴ{�b�N�X���擾���郁�\�b�h
	std::vector<Boxes*> GetAllBoxes() const
	{
		return boxes;
	}
	//�{�b�N�X�폜
	void Remove(Box* box);



private:
	//�{�b�N�X���m�̏Փˏ���
	void CollisionBoxVsBox();

	//�{�b�N�X�ƃv���C���[�̓����蔻��
	void CollisionBoxVsPlayer();

	//���ׂẴ{�b�N�X�ƃS�[���̓����蔻��
	void CollisionBoxesVsGoal();

	std::set<Box*> remove;

	std::vector<Box*> box;

	std::vector<Boxes*> boxes; // �{�b�N�X�̃��X�g
};

