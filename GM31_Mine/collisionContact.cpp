#include "myMath.h"
#include "collisionContact.h"

#define CONTACT_SAME_POINT (0.01f)
#define CONTACT_THRESHOLD_NORMAL (0.01f)	// �Փ˓_��臒l�i�@�������j
#define CONTACT_THRESHOLD_TANGENT (0.002f)	// �Փ˓_��臒l�i���ʏ�j

void CollisionContact::Init()
{
	m_numContacts = 0;
	for (int i = 0; i < COLLISION_CONTACT_POINT_NUM; i++) {
		m_contactPoints[i].Init();
	}
}

void CollisionContact::Update(D3DXVECTOR3& posA, D3DXQUATERNION& quatA, D3DXVECTOR3& posB, D3DXQUATERNION& quatB)
{
	// �Փ˓_�̍X�V
	// �Փ˓_�Ԃ̋��������l�𒴂����Ȃ�폜����
	for (int i = 0; i < m_numContacts; i++) {
		D3DXVECTOR3 normal = m_contactPoints[i].m_normal;
		D3DXVECTOR3 cPosA = posA + RotateVecForQuat(quatA, m_contactPoints[i].m_pointA);
		D3DXVECTOR3 cPosB = posB + RotateVecForQuat(quatB, m_contactPoints[i].m_pointB);

		// �ђʐ[�x���v���X�ɓ]�������ǂ����`�F�b�N
		float distance = D3DXVec3Dot(&cPosA, &cPosB);
		if (distance > CONTACT_THRESHOLD_NORMAL) {
			RemoveContactPoint(i);
			i--;
			continue;
		}
		m_contactPoints[i].m_distance = distance;

		// �[�x�������������ė��_�̋������`�F�b�N
		cPosA = cPosA - m_contactPoints[i].m_distance * normal;
		D3DXVECTOR3 vecAB = cPosA - cPosB;
		float distanceAB = D3DXVec3LengthSq(&vecAB);
		if (distanceAB > CONTACT_THRESHOLD_TANGENT) {
			RemoveContactPoint(i);
			i--;
			continue;
		}
	}
}

void CollisionContact::RemoveContactPoint(int index)
{
	m_contactPoints[index] = m_contactPoints[index - 1];
	m_numContacts--;
}
