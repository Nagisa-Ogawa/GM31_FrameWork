#include "myMath.h"
#include "collisionContact.h"

#define CONTACT_SAME_POINT (0.01f)
#define CONTACT_THRESHOLD_NORMAL (0.01f)	// 衝突点の閾値（法線方向）
#define CONTACT_THRESHOLD_TANGENT (0.002f)	// 衝突点の閾値（平面上）

void CollisionContact::Init()
{
	m_numContacts = 0;
	for (int i = 0; i < COLLISION_CONTACT_POINT_NUM; i++) {
		m_contactPoints[i].Init();
	}
}

void CollisionContact::Update(D3DXVECTOR3& posA, D3DXQUATERNION& quatA, D3DXVECTOR3& posB, D3DXQUATERNION& quatB)
{
	// 衝突点の更新
	// 衝突点間の距離が一定値を超えたなら削除する
	for (int i = 0; i < m_numContacts; i++) {
		D3DXVECTOR3 normal = m_contactPoints[i].m_normal;
		D3DXVECTOR3 cPosA = posA + RotateVecForQuat(quatA, m_contactPoints[i].m_pointA);
		D3DXVECTOR3 cPosB = posB + RotateVecForQuat(quatB, m_contactPoints[i].m_pointB);

		// 貫通深度がプラスに転じたかどうかチェック
		float distance = D3DXVec3Dot(&cPosA, &cPosB);
		if (distance > CONTACT_THRESHOLD_NORMAL) {
			RemoveContactPoint(i);
			i--;
			continue;
		}
		m_contactPoints[i].m_distance = distance;

		// 深度方向を除去して両点の距離をチェック
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
