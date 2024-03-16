#pragma once
#include <d3dx9math.h>
#include "collisionConstraint.h"

#define COLLISION_CONTACT_POINT_NUM (4)


//-----------------------------------------------------
// 衝突時の衝突情報を表すクラス
//------------------------------------------------------
class CollisionContact 
{
	int m_numContacts;		// 衝突の数
	float m_friction;		// 摩擦
	CollisionContactPoint m_contactPoints[COLLISION_CONTACT_POINT_NUM];		// 衝突点の配列
};

//-----------------------------------------------------
// 衝突情報の衝突点を表すクラス
//------------------------------------------------------
class CollisionContactPoint
{
	float m_distance;		// 貫通深度
	D3DXVECTOR3 m_pointA;	// 衝突点(剛体Aのローカル座標系)
	D3DXVECTOR3 m_pointB;	// 衝突点(剛体Bのローカル座標系)
	D3DXVECTOR3 m_normal;	// 衝突点の法線ベクトル(ワールド座標系)
	//拘束
	CollisionConstraint m_constraints[3];		// 拘束
};