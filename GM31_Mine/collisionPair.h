#pragma once
#include "collisionContact.h"

// �����蔻�������y�A�̎��
enum PAIR_TYPE
{
	PAIR_TYPE_NEW,	// �V�K
	PAIR_TYPE_KEEP,	// �ێ�
};


//-----------------------------------------------------
// �����蔻�������(�Փ˂���\���̍���)�y�A��\���N���X
//------------------------------------------------------
class CollisionPair
{
	PAIR_TYPE m_pairType;
	union
	{
		int m_key;	// �Փ˃y�A�̃L�[
		struct {
			// �Փ˃y�A��ID 
			int m_rigidBodyAID;
			int m_rigidBodyBID;
		};
	};
	CollisionContact* m_contact;	// �Փˏ��
};
