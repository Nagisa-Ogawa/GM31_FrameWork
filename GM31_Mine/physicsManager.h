#pragma once

#include <vector>
#include "collisionPair.h"

// �V�~�����[�V�����萔��`
#define MAX_RIGIDBODIES	(500)
#define MAX_PAIRS			(5000)
#define CONTACT_BIAS		(0.1f)
#define CONTACT_SLOP		(0.001f)
#define ITERATION			(10)

class Transform;
class RigidBody;
class BoxCollision;

//-----------------------------------------------------
// �����V�~�����[�V�����Ǘ��p�N���X
//------------------------------------------------------
class PhysicsManager
{
private:

	// ���̂̃f�[�^
	Transform* m_states[MAX_RIGIDBODIES];	// ���̂̏��
	RigidBody* m_bodies[MAX_RIGIDBODIES];	// ���̂̑���
	BoxCollision* m_colliders[MAX_RIGIDBODIES];	// ���̂̌`��
	int m_numRigidBodies = 0;		// ���݂̍��̂̐�

	// �����蔻��p�y�A�̃f�[�^
	int m_pairSwap;
	int m_numPairs[2];
	CollisionPair m_pairs[2][MAX_PAIRS];	// ���������\���̂���y�A

	// �����V�~�����[�V�����p�֐�
	void AddExternalForce();	// �O�͂�^����֐�
	void BroadPhase();			// �u���[�h�t�F�[�Y�iAABB�œ������Ă������ȃy�A�����m����)�����s����֐�
	void NarrowPhase();			// �i���[�t�F�[�Y (�u���[�h�t�F�[�Y�ō쐬�����y�A�œ����蔻�������j�����s����֐�
	void SolveConstraint();		// �S���\���o�[ (�Փ˂��Ă��鍄�̂𔽔�������ׂ̌v�Z) ���Z�o 
	void UpdateTransform();		// ���̏����X�V

	// �����蔻��p�֐�
	bool Collision_AABBToAABB(D3DXVECTOR3 centerA, D3DXVECTOR3 halfA, D3DXVECTOR3 centerB, D3DXVECTOR3 halfB);

	// �\�[�g�p�֐�
	void SortPairByKey(CollisionPair* pairs, CollisionPair* sortBuff, int pairNum);		// �Փ˃y�A�̔z���key�̏����Ń\�[�g����֐�

	static PhysicsManager* m_Instance;	// �V���O���g���p�C���X�^���X
	PhysicsManager();	// �R���X�g���N�^
	PhysicsManager(const PhysicsManager& manager);	// �R�s�[�R���X�g���N�^
	PhysicsManager& operator=(const PhysicsManager& manager);	// ������Z�q

public:

	void Init();	// ����������
	void CreatePhysicsScene();
	void PhysicsSimulate();		// �����V�~�����[�V����
	void DestroyPhysicsScene();
	void Uninit();	// �I������


	virtual ~PhysicsManager();	// �f�X�g���N�^
	static PhysicsManager* GetInstance();


};
