#pragma once
#include <d3dx9math.h>
#include "collisionConstraint.h"

#define COLLISION_CONTACT_POINT_NUM (4)


//-----------------------------------------------------
// �Փˏ��̏Փ˓_��\���N���X
//------------------------------------------------------
class CollisionContactPoint
{
public:
	float m_distance;		// �ђʐ[�x
	D3DXVECTOR3 m_pointA;	// �Փ˓_(����A�̃��[�J�����W�n)
	D3DXVECTOR3 m_pointB;	// �Փ˓_(����B�̃��[�J�����W�n)
	D3DXVECTOR3 m_normal;	// �Փ˓_�̖@���x�N�g��(���[���h���W�n)
	//�S��
	CollisionConstraint m_constraints[3];		// �S��

	void Init() {
		m_constraints[0].m_accumImpulse = 0.0f;
		m_constraints[1].m_accumImpulse = 0.0f;
		m_constraints[2].m_accumImpulse = 0.0f;
	}
};

//-----------------------------------------------------
// �Փˎ��̏Փˏ���\���N���X
//------------------------------------------------------
class CollisionContact
{
public:
	int m_numContacts;		// �Փ˂̐�
	float m_friction;		// ���C
	CollisionContactPoint m_contactPoints[COLLISION_CONTACT_POINT_NUM];		// �Փ˓_�̔z��

	void Init();	// �������֐�
	void Update(D3DXVECTOR3& posA, D3DXQUATERNION& quatA, D3DXVECTOR3& posB, D3DXQUATERNION& quatB);	// �Փ˓_���X�V����
	void RemoveContactPoint(int index);
};