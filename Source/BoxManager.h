#pragma once
#include <vector>
#include <set>
#include "Box.h"
#include "Boxes.h"
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
	int GetBoxCount() const { return static_cast<int>(boxes.size()); }

	//�{�b�N�X�擾
	Box* GetBox(int index) const { return boxes.at(index); }

	//�{�b�N�X�폜
	void Remove(Box* box);



private:
	//�{�b�N�X���m�̏Փˏ���
	void CollisionBoxVsBox();

	
	//�{�b�N�X�ƃv���C���[�̓����蔻��
	void CollisionBoxVsPlayer();
	//�Փ˔���
	//void InstarsecCollision(Boxes* other);

	std::set<Box*> remove;

	std::vector<Box*> boxes;


};

