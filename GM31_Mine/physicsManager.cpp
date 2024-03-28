
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

#define MAX_VELOCITY (340)  // 並進運動の最大値
#define MAX_ANGULAR_VELOCITY (D3DX_PI * 60.0f)  // 回転運動の最大値
#define ADD_AABB_EXPANSION (0.01f)  // AABBサイズを少し拡張するための定数

PhysicsManager* PhysicsManager::m_Instance = NULL;



PhysicsManager::PhysicsManager()
{
    // なにもしない
}


PhysicsManager::PhysicsManager(const PhysicsManager& manager)
{
    // インスタンスをコピー
    m_Instance = manager.m_Instance;
}


PhysicsManager& PhysicsManager::operator=(const PhysicsManager& manager)
{
    // インスタンスをコピー
    m_Instance = manager.m_Instance;
    return *m_Instance;
}


/// <summary>
/// 初期化処理
/// </summary>
void PhysicsManager::Init()
{
}


/// <summary>
/// 物理シミュレーションを行うシーンを作成する関数
/// </summary>
void PhysicsManager::CreatePhysicsScene()
{
    // パラメータを初期化
    m_numRigidBodies = 0;
    m_numPairs[0] = m_numPairs[1] = 0;
    m_pairSwap = 0;

    // 剛体コンポーネントを持つゲームオブジェクトから剛体データを取得
    auto objects = Manager::GetInstance()->GetScene()->GetAllGameObjects();
    for (auto object : objects) {
        auto body = object->GetComponent<RigidBody>();
        // 剛体コンポーネントを持っていないなら無視
        if (body==nullptr) {
            continue;
        }
        // 剛体データを取得
        auto state = object->GetComponent<Transform>();
        auto collider = object->GetComponent<BoxCollision>();
        m_states[m_numRigidBodies] = state;
        m_bodies[m_numRigidBodies] = body;
        m_colliders[m_numRigidBodies] = collider;
        // 慣性テンソルを算出
        body->SetInertia(collider->CalcInertiaBox(body->GetMass()));
        m_numRigidBodies++;
        // 剛体を囲むAABBのサイズを算出
        D3DXVECTOR3 center, half;
        collider->GetCollisionMesh()->CreateMeshAABB(collider->GetOffset(), state->m_localQuaternion, &center, &half);
        collider->SetBFCenter(center);
        collider->SetBFHalf(half);
    }
}


/// <summary>
/// 物理シミュレーションを行う関数
/// </summary>
void PhysicsManager::PhysicsSimulate()
{
    // 前フレームとは違う配列を使用する
    m_pairSwap = 1 - m_pairSwap;

    // 外力を与える
    AddExternalForce();
    // ブロードフェーズ (AABBで当たっていそうなペアを検知する）を実行
    BroadPhase();
    // ナローフェーズ (ブロードフェーズで作成したペアで当たり判定をする）を実行 

    // 拘束ソルバー (衝突している剛体を反発させる為の計算) を算出 

    // 剛体情報を更新
    UpdateTransform();
}


/// <summary>
/// 外力を与える関数
/// </summary>
void PhysicsManager::AddExternalForce()
{
    for (int i = 0; i < m_numRigidBodies; i++) {
        RigidBody* body = m_bodies[i];
        Transform* state = m_states[i];
        // 剛体のタイプがstaticなら外力は与えない
        if (body->GetMotionType() == MOTION_TYPE::STATIC) {
            continue;
        }

        // 外力
        D3DXVECTOR3 externalForce = body->GetGravity() * body->GetMass();   // 重力を与える
        D3DXVECTOR3 externalTorque = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 角速度はなし
        // この剛体の姿勢（クォータニオンを回転行列へ）
        D3DXMATRIX orientation;
        D3DXMatrixRotationQuaternion(&orientation, &state->m_localQuaternion);
        // 剛体の慣性テンソルをワールド座標系へ
        // 慣性テンソルをの回転 = R×I×R^T (回転行列×慣性テンソル×回転行列の転置行列
        D3DXMATRIX inertia = body->GetInertia();
        D3DXMATRIX tOrientation; D3DXMatrixTranspose(&tOrientation, &orientation);
        D3DXMATRIX worldInertia = orientation * inertia * tOrientation;
        // 慣性テンソルの逆行列
        D3DXMATRIX invInertia; D3DXMatrixInverse(&invInertia, NULL, &inertia);
        D3DXMATRIX worldInvInertia = orientation * invInertia * tOrientation;
        // トルクを算出
        // トルク = 慣性テンソル×角加速度
        D3DXVECTOR3 torque = MulMatVec(worldInertia, body->GetAngularVelocity());

        // 外力を並進速度へ反映
        // 並進速度 = 外力 ÷ 質量 × t
        D3DXVECTOR3 velocity = body->GetVelocity();
        velocity += externalForce / body->GetMass() * TIME_STEP;
        body->SetVelocity(velocity);

        // 外力を回転運動へ反映
        // 角加速度 = 慣性テンソルの逆行列×トルク
        D3DXVECTOR3 angularVelocity = body->GetAngularVelocity();
        torque += externalTorque * TIME_STEP;
        angularVelocity = MulMatVec(worldInvInertia, torque);

        // 最大値チェック
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
/// ブロードフェーズ (AABBで当たっていそうなペアを検知する）を実行
/// </summary>
void PhysicsManager::BroadPhase()
{
    // 前フレームの衝突ペア情報が入った配列
    CollisionPair* oldPairs = m_pairs[1 - m_pairSwap];
    int& numOldPairs = m_numPairs[1 - m_pairSwap];
    // このフレームで衝突ペア情報を入れる配列
    CollisionPair* newPairs = m_pairs[m_pairSwap];
    int& numNewPairs = m_numPairs[m_pairSwap];

    // ペア数をリセット
    numNewPairs = 0;

    // AABBで当たってそうな見つけ、その剛体でペアクラスを作成
    // 注意！！　現在は総当たり（高速化手法実装予定）
    for (int i = 0; i < m_numRigidBodies; i++) {
        for (int j = i + 1; j < m_numRigidBodies; j++) {
            Transform* stateA = m_states[i];
            BoxCollision* colliderA = m_colliders[i];
            Transform* stateB = m_states[j];
            BoxCollision* colliderB = m_colliders[j];

            // AABBの中心座標をワールド座標系へ
            D3DXMATRIX orientationA; D3DXMatrixRotationQuaternion(&orientationA, &stateA->m_localQuaternion);
            D3DXVECTOR3 centerA = stateA->m_localPosition + MulMatVec(orientationA, colliderA->GetBFCenter());
            D3DXVECTOR3 halfA = MulMatVec( AbsMat(orientationA),(colliderA->GetBFHalf() + D3DXVECTOR3(ADD_AABB_EXPANSION, ADD_AABB_EXPANSION, ADD_AABB_EXPANSION)));

            D3DXMATRIX orientationB; D3DXMatrixRotationQuaternion(&orientationB, &stateB->m_localQuaternion);
            D3DXVECTOR3 centerB = stateB->m_localPosition + MulMatVec(orientationB, colliderB->GetBFCenter());
            D3DXVECTOR3 halfB = MulMatVec(AbsMat(orientationB), (colliderB->GetBFHalf() + D3DXVECTOR3(ADD_AABB_EXPANSION, ADD_AABB_EXPANSION, ADD_AABB_EXPANSION)));

            // AABB同士衝突チェック
            if (Collision_AABBToAABB(centerA, halfA, centerB, halfB)) {
                // AABB同士が当たっていたなら詳細な衝突チェックをする為に衝突ペアクラスに登録
                CollisionPair& newPair = newPairs[numNewPairs];
                // 剛体配列の添え字が小さい方をAへ
                newPair.m_rigidBodyAID = i < j ? i : j;
                newPair.m_rigidBodyBID = i > j ? i : j;
                // 衝突情報は詳細な衝突検知をまだしていないため空
                newPair.m_contact = NULL;
            }
        }
    }

    // 衝突ペア配列をkeyの昇順でソート
    CollisionPair* buff = new CollisionPair[sizeof(CollisionPair) * numNewPairs];
    SortPairByKey(newPairs, buff, numNewPairs);
    delete[] buff;

    // 現在のフレームから衝突したペア用配列
    CollisionPair* outNewPairs = new CollisionPair[sizeof(CollisionPair) * numNewPairs];
    // 前のフレームと現在のフレームでも衝突しているペア用配列
    CollisionPair* outKeepPairs = new CollisionPair[sizeof(CollisionPair) * numOldPairs];
    int outNewCount = 0, outKeepCount = 0;
    int newId = 0, oldId = 0;

    // 前フレームと現在のフレームのどちらでも衝突が起きていたなら
    // もう衝突していないペア、まだ衝突しているペア、新しく衝突したペアの3種類に分ける
    while (oldId < numOldPairs && newId < numNewPairs) {
        // 新しい衝突ペアよりも小さいkeyのペアは存在しないのでもう衝突していないペア
        if (newPairs[newId].m_key > oldPairs[oldId].m_key) {
            // 衝突情報を解放
            delete oldPairs[oldId].m_contact;
            oldId++;
        }
        // 同じkeyのペアがあるなら前のフレームから現在のフレームまで衝突しているペア
        if (newPairs[newId].m_key == oldPairs[oldId].m_key) {
            outKeepPairs[outKeepCount] = oldPairs[oldId];
            outKeepCount++;
            oldId++;
            newId++;
        }
        // 他は全て現在のフレームから衝突したペア
        else {
            outNewPairs[outNewCount] = newPairs[newId];
            outNewCount++;
            newId++;
        }
    }

    // 一番初めのフレームなら全てがこのフレームから衝突
    if (newId < numNewPairs) {
        while (newId < numNewPairs) {
            outNewPairs[outNewCount] = newPairs[newId];
            outNewCount++;
            newId++;
        }
    }
    // 他の前フレームのペアはすべて廃棄
    else if (oldId < numOldPairs) {
        while (oldId < numOldPairs) {
            delete oldPairs[oldId].m_contact;
            oldId++;
        }
    }

    // このフレームから衝突したペアの衝突情報を作成・初期化
    for (int i = 0; i < outNewCount; i++) {
        outNewPairs[i].m_contact = new CollisionContact();
        outNewPairs[i].m_contact->Init();
    }

    // 前フレームから衝突し続けているペアは情報を更新
    for (int i = 0; i < outKeepCount; i++) {
        outKeepPairs[i].m_contact->Update(
            m_states[outKeepPairs[i].m_rigidBodyAID]->m_localPosition,
            m_states[outKeepPairs[i].m_rigidBodyAID]->m_localQuaternion,
            m_states[outKeepPairs[i].m_rigidBodyBID]->m_localPosition,
            m_states[outKeepPairs[i].m_rigidBodyBID]->m_localQuaternion
        );
    }

    numNewPairs = 0;
    // 現在のフレームの衝突ペア配列に格納
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

    // 衝突ペア配列をkeyの昇順でソート
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
/// 剛体の状態を更新する関数
/// </summary>
void PhysicsManager::UpdateTransform()
{
    for (int i = 0; i < m_numRigidBodies; i++) {
        RigidBody* body = m_bodies[i];
        Transform* state = m_states[i];
        D3DXVECTOR3 angVel = body->GetAngularVelocity();
        D3DXQUATERNION dragAng = D3DXQUATERNION(angVel.x, angVel.y, angVel.z, 0) * state->m_localQuaternion * 0.5f;

        // 並進運動
        state->m_localPosition += body->GetVelocity() * TIME_STEP;
        // 回転運動
        D3DXQUATERNION qua = state->m_localQuaternion + dragAng * TIME_STEP;
        D3DXQuaternionNormalize(&state->m_localQuaternion, &qua);
    }
}


/// <summary>
/// AABB同士の当たり判定をする関数
/// </summary>
/// <param name="centerA">AABB(A)の中心座標</param>
/// <param name="halfA">AABB(A)のサイズの半分</param>
/// <param name="centerB">AABB(B)の中心座標</param>
/// <param name="halfB">AABB(B)のサイズの半分</param>
/// <returns>当たったかどうか</returns>
bool PhysicsManager::Collision_AABBToAABB(D3DXVECTOR3 centerA, D3DXVECTOR3 halfA, D3DXVECTOR3 centerB, D3DXVECTOR3 halfB)
{
    if (fabsf(centerA.x - centerB.x) > halfA.x + halfB.x) return false;
    if (fabsf(centerA.y - centerB.y) > halfA.y + halfB.y) return false;
    if (fabsf(centerA.z - centerB.z) > halfA.z + halfB.z)return false;
    return true;
}


/// <summary>
/// 衝突ペアをkeyでソートする関数
/// </summary>
/// <param name="pairs"></param>
/// <param name="sortBuff"></param>
/// <param name="pairNum"></param>
void PhysicsManager::SortPairByKey(CollisionPair* pairs, CollisionPair* sortBuff, int pairNum)
{
    // 配列を真ん中で分割
    int left = pairNum >> 1;
    int right = pairNum - left;

    // まだ分割出来るなら分割
    if (left > 1) {
        SortPairByKey(pairs, sortBuff, left);
    }
    if (right > 1) {
        SortPairByKey(pairs, sortBuff, right);
    }

    // 真ん中より右と左の配列に分ける
    CollisionPair* leftPairs = pairs;
    CollisionPair* rightPairs = pairs + left;

    int leftCount = 0, rightCount = 0;

    // 左右どちらかの配列を全て比較し終わるまで
    while (leftCount < left && rightCount < right) {
        // keyが小さい方をバッファに入れる
        if (leftPairs[leftCount].m_key < rightPairs[rightCount].m_key) {
            sortBuff[leftCount + rightCount] = leftPairs[leftCount];
            leftCount++;
        }
        else {
            sortBuff[leftCount + rightCount] = rightPairs[rightCount];
            rightCount++;
        }
    }

    // 比較していない要素があったならバッファに格納
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

    // ソートしたバッファから元の配列に格納
    for (int i = 0; i < left + right; i++) {
        pairs[i] = sortBuff[i];
    }
}


void PhysicsManager::DestroyPhysicsScene()
{
}


// 終了処理
void PhysicsManager::Uninit()
{
}


PhysicsManager::~PhysicsManager()
{
    // インスタンスを解放
    delete m_Instance;
}


/// <summary>
/// PhysicsManagerの実体を渡す関数
/// </summary>
/// <returns>実体</returns>
PhysicsManager* PhysicsManager::GetInstance()
{
    // 初めて使うときにインスタンスを生成
// それ以降は生成したインスタンスを渡す
    if (m_Instance == NULL)
    {
        m_Instance = new PhysicsManager();
    }
    return m_Instance;

}
