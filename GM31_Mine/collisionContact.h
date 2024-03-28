#pragma once
#include <d3dx9math.h>
#include "collisionConstraint.h"

#define COLLISION_CONTACT_POINT_NUM (4)


//-----------------------------------------------------
// 衝突情報の衝突点を表すクラス
//------------------------------------------------------
class CollisionContactPoint
{
public:
	float m_distance;		// 貫通深度
	D3DXVECTOR3 m_pointA;	// 衝突点(剛体Aのローカル座標系)
	D3DXVECTOR3 m_pointB;	// 衝突点(剛体Bのローカル座標系)
	D3DXVECTOR3 m_normal;	// 衝突点の法線ベクトル(ワールド座標系)
	//拘束
	CollisionConstraint m_constraints[3];		// 拘束

	void Init() {
		m_constraints[0].m_accumImpulse = 0.0f;
		m_constraints[1].m_accumImpulse = 0.0f;
		m_constraints[2].m_accumImpulse = 0.0f;
	}
};

//-----------------------------------------------------
// 衝突時の衝突情報を表すクラス
//------------------------------------------------------
class CollisionContact
{
public:
	int m_numContacts;		// 衝突の数
	float m_friction;		// 摩擦
	CollisionContactPoint m_contactPoints[COLLISION_CONTACT_POINT_NUM];		// 衝突点の配列

	void Init();	// 初期化関数
	void Update(D3DXVECTOR3& posA, D3DXQUATERNION& quatA, D3DXVECTOR3& posB, D3DXQUATERNION& quatB);	// 衝突点を更新する
	void RemoveContactPoint(int index);
};