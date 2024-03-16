#pragma once
#include <d3dx9math.h>
#include "collisionConstraint.h"

#define COLLISION_CONTACT_POINT_NUM (4)


//-----------------------------------------------------
// �Փˎ��̏Փˏ���\���N���X
//------------------------------------------------------
class CollisionContact 
{
	int m_numContacts;		// �Փ˂̐�
	float m_friction;		// ���C
	CollisionContactPoint m_contactPoints[COLLISION_CONTACT_POINT_NUM];		// �Փ˓_�̔z��
};

//-----------------------------------------------------
// �Փˏ��̏Փ˓_��\���N���X
//------------------------------------------------------
class CollisionContactPoint
{
	float m_distance;		// �ђʐ[�x
	D3DXVECTOR3 m_pointA;	// �Փ˓_(����A�̃��[�J�����W�n)
	D3DXVECTOR3 m_pointB;	// �Փ˓_(����B�̃��[�J�����W�n)
	D3DXVECTOR3 m_normal;	// �Փ˓_�̖@���x�N�g��(���[���h���W�n)
	//�S��
	CollisionConstraint m_constraints[3];		// �S��
};