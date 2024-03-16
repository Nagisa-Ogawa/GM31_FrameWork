#pragma once
#include "collisionContact.h"

// 当たり判定をするペアの種類
enum PAIR_TYPE
{
	PAIR_TYPE_NEW,	// 新規
	PAIR_TYPE_KEEP,	// 維持
};


//-----------------------------------------------------
// 当たり判定をする(衝突する可能性の高い)ペアを表すクラス
//------------------------------------------------------
class CollisionPair
{
	PAIR_TYPE m_pairType;
	union
	{
		int m_key;	// 衝突ペアのキー
		struct {
			// 衝突ペアのID 
			int m_rigidBodyAID;
			int m_rigidBodyBID;
		};
	};
	CollisionContact* m_contact;	// 衝突情報
};
