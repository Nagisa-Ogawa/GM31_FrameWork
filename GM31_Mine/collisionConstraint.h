#pragma once
#include <d3dx9math.h>


//-----------------------------------------------------
// �Փˎ��̍S����\���N���X
//------------------------------------------------------
class CollisionConstraint
{
	D3DXVECTOR3 m_axis;		// �S����
	float m_cnExpDenom;		// �S�����̕���
	float m_initCnImpulse;	// �����S����
	float m_lowerLimit;		// �S���͂̉���
	float m_upperLimit;		// �S���͂̏��
	float m_accumImpulse;	// �~�ς����S����
};
