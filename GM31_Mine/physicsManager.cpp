
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "physicsManager.h"
#include "transform.h"
#include "rigidBody.h"
#include "boxCollision.h"

PhysicsManager* PhysicsManager::m_Instance = NULL;


PhysicsManager::PhysicsManager()
{
    // �Ȃɂ����Ȃ�
}


PhysicsManager::PhysicsManager(const PhysicsManager& manager)
{
    // �C���X�^���X���R�s�[
    m_Instance = manager.m_Instance;
}


PhysicsManager& PhysicsManager::operator=(const PhysicsManager& manager)
{
    // �C���X�^���X���R�s�[
    m_Instance = manager.m_Instance;
    return *m_Instance;
}


/// <summary>
/// ����������
/// </summary>
void PhysicsManager::Init()
{
}


/// <summary>
/// �����V�~�����[�V�������s���V�[�����쐬����֐�
/// </summary>
void PhysicsManager::CreatePhysicsScene()
{
    // �p�����[�^��������
    m_numRigidBodies = 0;
    m_numPairs[0] = m_numPairs[1] = 0;
    m_pairSwap = 0;

    // ���̃R���|�[�l���g�����Q�[���I�u�W�F�N�g���獄�̃f�[�^���擾
    auto objects = Manager::GetInstance()->GetScene()->GetAllGameObjects();
    for (auto object : objects) {
        auto body = object->GetComponent<RigidBody>();
        // ���̃R���|�[�l���g�������Ă��Ȃ��Ȃ疳��
        if (body==nullptr) {
            continue;
        }
        // ���̃f�[�^���擾
        auto state = object->GetComponent<Transform>();
        auto collider = object->GetComponent<BoxCollision>();
        m_states.push_back(state);
        m_bodies.push_back(body);
        m_colliders.push_back(collider);
        // �����e���\�����Z�o
        body->SetInertia(collider->CalcInertiaBox(body->GetMass()));
        m_numRigidBodies++;
    }
}


/// <summary>
/// �����V�~�����[�V�������s���֐�
/// </summary>
void PhysicsManager::PhysicsSimulate()
{
    // �O�͂�^����

    // �u���[�h�t�F�[�Y (AABB�œ������Ă������ȃy�A�����m����j�����s

    // �i���[�t�F�[�Y (�u���[�h�t�F�[�Y�ō쐬�����y�A�œ����蔻�������j�����s 

    // �S���\���o�[ (�Փ˂��Ă��鍄�̂𔽔�������ׂ̌v�Z) ���Z�o 

    // ���̏����X�V
}


void PhysicsManager::AddExternalForce()
{
}

void PhysicsManager::BroadPhase()
{
}

void PhysicsManager::NarrowPhase()
{
}

void PhysicsManager::SolveConstraint()
{
}

void PhysicsManager::UpdateTransform()
{
}

void PhysicsManager::DestroyPhysicsScene()
{
}


// �I������
void PhysicsManager::Uninit()
{
}


PhysicsManager::~PhysicsManager()
{
    // �C���X�^���X�����
    delete m_Instance;
}


/// <summary>
/// PhysicsManager�̎��̂�n���֐�
/// </summary>
/// <returns>����</returns>
PhysicsManager* PhysicsManager::GetInstance()
{
    // ���߂Ďg���Ƃ��ɃC���X�^���X�𐶐�
// ����ȍ~�͐��������C���X�^���X��n��
    if (m_Instance == NULL)
    {
        m_Instance = new PhysicsManager();
    }
    return m_Instance;

}
