#pragma once

#include <list>

class BoxCollision;
class QuadCollision;

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
	std::list<BoxCollision*> m_BoxCollList;
	std::list<QuadCollision*> m_QuadCollList;
	// �g���K�[�p���X�g
	std::list<BoxCollision*> m_BoxTriList;
	std::list<QuadCollision*> m_QuadTriList;


	// ����������
	void Init();
	// ���e�����̔����̒������擾����֐�
	float CreateHalfProjectionLine(D3DXVECTOR3* sAxis, D3DXVECTOR3* vecX, D3DXVECTOR3* vecY, D3DXVECTOR3* vecZ = 0);
public:
	// �f�X�g���N�^
	virtual ~CollisionManager();
	static CollisionManager* GetInstance();

	void AddBoxCollision(BoxCollision* coll);
	void AddQuadCollision(QuadCollision* coll);
	bool Collision_BoxToBox(BoxCollision* a, BoxCollision* b);
	bool Collision_BoxToQuad(BoxCollision* a, QuadCollision* b, float* l,D3DXVECTOR3* dir);

};