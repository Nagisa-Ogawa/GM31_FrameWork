
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
        m_states.push_back(state);
        m_bodies.push_back(body);
        m_colliders.push_back(collider);
        // 慣性テンソルを算出
        body->SetInertia(collider->CalcInertiaBox(body->GetMass()));
        m_numRigidBodies++;
    }
}


/// <summary>
/// 物理シミュレーションを行う関数
/// </summary>
void PhysicsManager::PhysicsSimulate()
{
    // 外力を与える

    // ブロードフェーズ (AABBで当たっていそうなペアを検知する）を実行

    // ナローフェーズ (ブロードフェーズで作成したペアで当たり判定をする）を実行 

    // 拘束ソルバー (衝突している剛体を反発させる為の計算) を算出 

    // 剛体情報を更新
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
