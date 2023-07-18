#pragma once

#include <list>

class BoxCollision;

class CollisionManager
{
private:
	// �V���O���g���p�C���X�^���X
	static CollisionManager* m_Instance;
	// �R���X�g���N�^
	CollisionManager();
	// �R�s�[�R���X�g���N�^
	CollisionManager(const CollisionManager& manager);
	// ������Z�q
	CollisionManager& operator=(const CollisionManager& manager);

	// �����蔻�肠��p���X�g
	std::list<CollisionManager*> m_CollisionList;
	// �g���K�[�p���X�g
	std::list<CollisionManager*> m_TriggerList;


	// ����������
	void Init();
	// ���e�����̔����̒������擾����֐�
	float CreateHalfProjectionLine(D3DXVECTOR3* sAxis, D3DXVECTOR3* vecX, D3DXVECTOR3* vecY, D3DXVECTOR3* vecZ = 0);
public:
	// �f�X�g���N�^
	virtual ~CollisionManager();
	static CollisionManager* GetInstance();

	bool Collision_BoxToBox(BoxCollision* a, BoxCollision* b);

};