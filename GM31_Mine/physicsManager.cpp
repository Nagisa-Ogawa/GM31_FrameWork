
#include <algorithm>
#include "main.h"
#include "myMath.h"
#include "manager.h"
#include "scene.h"
#include "physicsManager.h"
#include "transform.h"
#include "rigidBody.h"
#include "boxCollision.h"
#include "collisionMesh.h"

#define MAX_VELOCITY (340)  // ���i�^���̍ő�l
#define MAX_ANGULAR_VELOCITY (D3DX_PI * 60.0f)  // ��]�^���̍ő�l
#define ADD_AABB_EXPANSION (0.01f)  // AABB�T�C�Y�������g�����邽�߂̒萔

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
        m_states[m_numRigidBodies] = state;
        m_bodies[m_numRigidBodies] = body;
        m_colliders[m_numRigidBodies] = collider;
        // �����e���\�����Z�o
        body->SetInertia(collider->CalcInertiaBox(body->GetMass()));
        m_numRigidBodies++;
        // ���̂��͂�AABB�̃T�C�Y���Z�o
        D3DXVECTOR3 center, half;
        collider->GetCollisionMesh()->CreateMeshAABB(collider->GetOffset(), state->m_localQuaternion, &center, &half);
        collider->SetBFCenter(center);
        collider->SetBFHalf(half);
    }
}


/// <summary>
/// �����V�~�����[�V�������s���֐�
/// </summary>
void PhysicsManager::PhysicsSimulate()
{
    // �O�t���[���Ƃ͈Ⴄ�z����g�p����
    m_pairSwap = 1 - m_pairSwap;

    // �O�͂�^����
    AddExternalForce();
    // �u���[�h�t�F�[�Y (AABB�œ������Ă������ȃy�A�����m����j�����s
    BroadPhase();
    // �i���[�t�F�[�Y (�u���[�h�t�F�[�Y�ō쐬�����y�A�œ����蔻�������j�����s 

    // �S���\���o�[ (�Փ˂��Ă��鍄�̂𔽔�������ׂ̌v�Z) ���Z�o 

    // ���̏����X�V
    UpdateTransform();
}


/// <summary>
/// �O�͂�^����֐�
/// </summary>
void PhysicsManager::AddExternalForce()
{
    for (int i = 0; i < m_numRigidBodies; i++) {
        RigidBody* body = m_bodies[i];
        Transform* state = m_states[i];
        // ���̂̃^�C�v��static�Ȃ�O�͂͗^���Ȃ�
        if (body->GetMotionType() == MOTION_TYPE::STATIC) {
            continue;
        }

        // �O��
        D3DXVECTOR3 externalForce = body->GetGravity() * body->GetMass();   // �d�͂�^����
        D3DXVECTOR3 externalTorque = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �p���x�͂Ȃ�
        // ���̍��̂̎p���i�N�H�[�^�j�I������]�s��ցj
        D3DXMATRIX orientation;
        D3DXMatrixRotationQuaternion(&orientation, &state->m_localQuaternion);
        // ���̂̊����e���\�������[���h���W�n��
        // �����e���\�����̉�] = R�~I�~R^T (��]�s��~�����e���\���~��]�s��̓]�u�s��
        D3DXMATRIX inertia = body->GetInertia();
        D3DXMATRIX tOrientation; D3DXMatrixTranspose(&tOrientation, &orientation);
        D3DXMATRIX worldInertia = orientation * inertia * tOrientation;
        // �����e���\���̋t�s��
        D3DXMATRIX invInertia; D3DXMatrixInverse(&invInertia, NULL, &inertia);
        D3DXMATRIX worldInvInertia = orientation * invInertia * tOrientation;
        // �g���N���Z�o
        // �g���N = �����e���\���~�p�����x
        D3DXVECTOR3 torque = MulMatVec(worldInertia, body->GetAngularVelocity());

        // �O�͂���i���x�֔��f
        // ���i���x = �O�� �� ���� �~ t
        D3DXVECTOR3 velocity = body->GetVelocity();
        velocity += externalForce / body->GetMass() * TIME_STEP;
        body->SetVelocity(velocity);

        // �O�͂���]�^���֔��f
        // �p�����x = �����e���\���̋t�s��~�g���N
        D3DXVECTOR3 angularVelocity = body->GetAngularVelocity();
        torque += externalTorque * TIME_STEP;
        angularVelocity = MulMatVec(worldInvInertia, torque);

        // �ő�l�`�F�b�N
        float sqVel = D3DXVec3LengthSq(&velocity);
        if (sqVel > (MAX_VELOCITY * MAX_VELOCITY)) {
            velocity = (velocity / sqrtf(sqVel)) * MAX_VELOCITY;
            body->SetVelocity(velocity);
        }
        float sqAngVel = D3DXVec3LengthSq(&angularVelocity);
        if (sqAngVel > (MAX_ANGULAR_VELOCITY * MAX_ANGULAR_VELOCITY)) {
            angularVelocity = (angularVelocity / sqrtf(sqAngVel)) * MAX_ANGULAR_VELOCITY;
            body->SetAngularVelocity(angularVelocity);
        }
    }
}


/// <summary>
/// �u���[�h�t�F�[�Y (AABB�œ������Ă������ȃy�A�����m����j�����s
/// </summary>
void PhysicsManager::BroadPhase()
{
    // �O�t���[���̏Փ˃y�A��񂪓������z��
    CollisionPair* oldPairs = m_pairs[1 - m_pairSwap];
    int& numOldPairs = m_numPairs[1 - m_pairSwap];
    // ���̃t���[���ŏՓ˃y�A��������z��
    CollisionPair* newPairs = m_pairs[m_pairSwap];
    int& numNewPairs = m_numPairs[m_pairSwap];

    // �y�A�������Z�b�g
    numNewPairs = 0;

    // AABB�œ������Ă����Ȍ����A���̍��̂Ńy�A�N���X���쐬
    // ���ӁI�I�@���݂͑�������i��������@�����\��j
    for (int i = 0; i < m_numRigidBodies; i++) {
        for (int j = i + 1; j < m_numRigidBodies; j++) {
            Transform* stateA = m_states[i];
            BoxCollision* colliderA = m_colliders[i];
            Transform* stateB = m_states[j];
            BoxCollision* colliderB = m_colliders[j];

            // AABB�̒��S���W�����[���h���W�n��
            D3DXMATRIX orientationA; D3DXMatrixRotationQuaternion(&orientationA, &stateA->m_localQuaternion);
            D3DXVECTOR3 centerA = stateA->m_localPosition + MulMatVec(orientationA, colliderA->GetBFCenter());
            D3DXVECTOR3 halfA = MulMatVec( AbsMat(orientationA),(colliderA->GetBFHalf() + D3DXVECTOR3(ADD_AABB_EXPANSION, ADD_AABB_EXPANSION, ADD_AABB_EXPANSION)));

            D3DXMATRIX orientationB; D3DXMatrixRotationQuaternion(&orientationB, &stateB->m_localQuaternion);
            D3DXVECTOR3 centerB = stateB->m_localPosition + MulMatVec(orientationB, colliderB->GetBFCenter());
            D3DXVECTOR3 halfB = MulMatVec(AbsMat(orientationB), (colliderB->GetBFHalf() + D3DXVECTOR3(ADD_AABB_EXPANSION, ADD_AABB_EXPANSION, ADD_AABB_EXPANSION)));

            // AABB���m�Փ˃`�F�b�N
            if (Collision_AABBToAABB(centerA, halfA, centerB, halfB)) {
                // AABB���m���������Ă����Ȃ�ڍׂȏՓ˃`�F�b�N������ׂɏՓ˃y�A�N���X�ɓo�^
                CollisionPair& newPair = newPairs[numNewPairs];
                // ���̔z��̓Y����������������A��
                newPair.m_rigidBodyAID = i < j ? i : j;
                newPair.m_rigidBodyBID = i > j ? i : j;
                // �Փˏ��͏ڍׂȏՓˌ��m���܂����Ă��Ȃ����ߋ�
                newPair.m_contact = NULL;
            }
        }
    }

    // �Փ˃y�A�z���key�̏����Ń\�[�g
    CollisionPair* buff = new CollisionPair[sizeof(CollisionPair) * numNewPairs];
    SortPairByKey(newPairs, buff, numNewPairs);
    delete[] buff;

    // ���݂̃t���[������Փ˂����y�A�p�z��
    CollisionPair* outNewPairs = new CollisionPair[sizeof(CollisionPair) * numNewPairs];
    // �O�̃t���[���ƌ��݂̃t���[���ł��Փ˂��Ă���y�A�p�z��
    CollisionPair* outKeepPairs = new CollisionPair[sizeof(CollisionPair) * numOldPairs];
    int outNewCount = 0, outKeepCount = 0;
    int newId = 0, oldId = 0;

    // �O�t���[���ƌ��݂̃t���[���̂ǂ���ł��Փ˂��N���Ă����Ȃ�
    // �����Փ˂��Ă��Ȃ��y�A�A�܂��Փ˂��Ă���y�A�A�V�����Փ˂����y�A��3��ނɕ�����
    while (oldId < numOldPairs && newId < numNewPairs) {
        // �V�����Փ˃y�A����������key�̃y�A�͑��݂��Ȃ��̂ł����Փ˂��Ă��Ȃ��y�A
        if (newPairs[newId].m_key > oldPairs[oldId].m_key) {
            // �Փˏ������
            delete oldPairs[oldId].m_contact;
            oldId++;
        }
        // ����key�̃y�A������Ȃ�O�̃t���[�����猻�݂̃t���[���܂ŏՓ˂��Ă���y�A
        if (newPairs[newId].m_key == oldPairs[oldId].m_key) {
            outKeepPairs[outKeepCount] = oldPairs[oldId];
            outKeepCount++;
            oldId++;
            newId++;
        }
        // ���͑S�Č��݂̃t���[������Փ˂����y�A
        else {
            outNewPairs[outNewCount] = newPairs[newId];
            outNewCount++;
            newId++;
        }
    }

    // ��ԏ��߂̃t���[���Ȃ�S�Ă����̃t���[������Փ�
    if (newId < numNewPairs) {
        while (newId < numNewPairs) {
            outNewPairs[outNewCount] = newPairs[newId];
            outNewCount++;
            newId++;
        }
    }
    // ���̑O�t���[���̃y�A�͂��ׂĔp��
    else if (oldId < numOldPairs) {
        while (oldId < numOldPairs) {
            delete oldPairs[oldId].m_contact;
            oldId++;
        }
    }

    // ���̃t���[������Փ˂����y�A�̏Փˏ����쐬�E������
    for (int i = 0; i < outNewCount; i++) {
        outNewPairs[i].m_contact = new CollisionContact();
        outNewPairs[i].m_contact->Init();
    }

    // �O�t���[������Փ˂������Ă���y�A�͏����X�V
    for (int i = 0; i < outKeepCount; i++) {
        outKeepPairs[i].m_contact->Update(
            m_states[outKeepPairs[i].m_rigidBodyAID]->m_localPosition,
            m_states[outKeepPairs[i].m_rigidBodyAID]->m_localQuaternion,
            m_states[outKeepPairs[i].m_rigidBodyBID]->m_localPosition,
            m_states[outKeepPairs[i].m_rigidBodyBID]->m_localQuaternion
        );
    }

    numNewPairs = 0;
    // ���݂̃t���[���̏Փ˃y�A�z��Ɋi�[
    for (int i = 0; i < outKeepCount; i++) {
        outKeepPairs[i].m_pairType = PAIR_TYPE_KEEP;
        newPairs[numNewPairs] = outKeepPairs[i];
        numNewPairs++;
    }
    for (int i = 0; i < outNewCount; i++) {
        outNewPairs[i].m_pairType = PAIR_TYPE_NEW;
        newPairs[numNewPairs] = outNewPairs[i];
        numNewPairs++;
    }

    delete[] outNewPairs;
    delete[] outKeepPairs;

    // �Փ˃y�A�z���key�̏����Ń\�[�g
    CollisionPair* buff = new CollisionPair[sizeof(CollisionPair) * numNewPairs];
    SortPairByKey(newPairs, buff, numNewPairs);
    delete[] buff;

}

void PhysicsManager::NarrowPhase()
{
}

void PhysicsManager::SolveConstraint()
{   
}


/// <summary>
/// ���̂̏�Ԃ��X�V����֐�
/// </summary>
void PhysicsManager::UpdateTransform()
{
    for (int i = 0; i < m_numRigidBodies; i++) {
        RigidBody* body = m_bodies[i];
        Transform* state = m_states[i];
        D3DXVECTOR3 angVel = body->GetAngularVelocity();
        D3DXQUATERNION dragAng = D3DXQUATERNION(angVel.x, angVel.y, angVel.z, 0) * state->m_localQuaternion * 0.5f;

        // ���i�^��
        state->m_localPosition += body->GetVelocity() * TIME_STEP;
        // ��]�^��
        D3DXQUATERNION qua = state->m_localQuaternion + dragAng * TIME_STEP;
        D3DXQuaternionNormalize(&state->m_localQuaternion, &qua);
    }
}


/// <summary>
/// AABB���m�̓����蔻�������֐�
/// </summary>
/// <param name="centerA">AABB(A)�̒��S���W</param>
/// <param name="halfA">AABB(A)�̃T�C�Y�̔���</param>
/// <param name="centerB">AABB(B)�̒��S���W</param>
/// <param name="halfB">AABB(B)�̃T�C�Y�̔���</param>
/// <returns>�����������ǂ���</returns>
bool PhysicsManager::Collision_AABBToAABB(D3DXVECTOR3 centerA, D3DXVECTOR3 halfA, D3DXVECTOR3 centerB, D3DXVECTOR3 halfB)
{
    if (fabsf(centerA.x - centerB.x) > halfA.x + halfB.x) return false;
    if (fabsf(centerA.y - centerB.y) > halfA.y + halfB.y) return false;
    if (fabsf(centerA.z - centerB.z) > halfA.z + halfB.z)return false;
    return true;
}


/// <summary>
/// �Փ˃y�A��key�Ń\�[�g����֐�
/// </summary>
/// <param name="pairs"></param>
/// <param name="sortBuff"></param>
/// <param name="pairNum"></param>
void PhysicsManager::SortPairByKey(CollisionPair* pairs, CollisionPair* sortBuff, int pairNum)
{
    // �z���^�񒆂ŕ���
    int left = pairNum >> 1;
    int right = pairNum - left;

    // �܂������o����Ȃ番��
    if (left > 1) {
        SortPairByKey(pairs, sortBuff, left);
    }
    if (right > 1) {
        SortPairByKey(pairs, sortBuff, right);
    }

    // �^�񒆂��E�ƍ��̔z��ɕ�����
    CollisionPair* leftPairs = pairs;
    CollisionPair* rightPairs = pairs + left;

    int leftCount = 0, rightCount = 0;

    // ���E�ǂ��炩�̔z���S�Ĕ�r���I���܂�
    while (leftCount < left && rightCount < right) {
        // key�������������o�b�t�@�ɓ����
        if (leftPairs[leftCount].m_key < rightPairs[rightCount].m_key) {
            sortBuff[leftCount + rightCount] = leftPairs[leftCount];
            leftCount++;
        }
        else {
            sortBuff[leftCount + rightCount] = rightPairs[rightCount];
            rightCount++;
        }
    }

    // ��r���Ă��Ȃ��v�f���������Ȃ�o�b�t�@�Ɋi�[
    if (leftCount < left) {
        while (leftCount < left) {
            sortBuff[leftCount + rightCount] = leftPairs[leftCount];
            leftCount++;
        }
    }
    if (rightCount < right) {
        while (rightCount < right) {
            sortBuff[leftCount + rightCount] = rightPairs[rightCount];
            rightCount++;
        }
    }

    // �\�[�g�����o�b�t�@���猳�̔z��Ɋi�[
    for (int i = 0; i < left + right; i++) {
        pairs[i] = sortBuff[i];
    }
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
