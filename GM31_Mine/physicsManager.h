#pragma once

#include <vector>

// �V�~�����[�V�����萔��`
#define MAX_RIGID_BODIES	(500)
#define MAX_PAIRS			(5000)
#define TIME_STEP			(0.016f)
#define CONTACT_BIAS		(0.1f)
#define CONTACT_SLOP		(0.001f)
#define ITERATION			(10)

class Transform;
class RigidBody;
class BoxCollision;
class CollisionPair;

//-----------------------------------------------------
// �����V�~�����[�V�����Ǘ��p�N���X
//------------------------------------------------------
class PhysicsManager
{
private:

	// ���̂̃f�[�^
	std::vector<Transform*>		m_states;	// ���̂̏��
	std::vector<RigidBody*>		m_bodies;	// ���̂̑���
	std::vector<BoxCollision*>	m_colliders;	// ���̂̌`��
	int m_numRigidBodies = 0;		// ���݂̍��̂̐�

	// �����蔻��p�y�A�̃f�[�^
	int m_pairSwap;
	int m_numPairs[2];
	std::vector<CollisionPair*[2]> m_pair;	// ���������\���̂���y�A

	// �����V�~�����[�V�����p�֐�
	void AddExternalForce();	// �O�͂�^����֐�
	void BroadPhase();			// �u���[�h�t�F�[�Y�iAABB�œ������Ă������ȃy�A�����m����)�����s����֐�
	void NarrowPhase();			// �i���[�t�F�[�Y (�u���[�h�t�F�[�Y�ō쐬�����y�A�œ����蔻�������j�����s����֐�
	void SolveConstraint();		// �S���\���o�[ (�Փ˂��Ă��鍄�̂𔽔�������ׂ̌v�Z) ���Z�o 
	void UpdateTransform();		// ���̏����X�V



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
