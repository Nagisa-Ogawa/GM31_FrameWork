#include <algorithm>
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "enemyFactory.h"
#include "enemy.h"
#include "model.h"

EnemyFactory* EnemyFactory::m_Instance = NULL;

EnemyFactory::EnemyFactory()
{
    // なにもしない
}

EnemyFactory::EnemyFactory(const EnemyFactory& manager)
{
    // インスタンスをコピー
    m_Instance = manager.m_Instance;
}

EnemyFactory& EnemyFactory::operator=(const EnemyFactory& manager)
{
    // インスタンスをコピー
    m_Instance = manager.m_Instance;
    return *m_Instance;
}

void EnemyFactory::Init()
{
    Model model;
    m_pModel = new MODEL();
    model.LoadObj("asset\\model\\box.obj", m_pModel);
}

EnemyFactory::~EnemyFactory()
{
    delete[] m_pModel->VertexArray;
    delete[] m_pModel->IndexArray;
    delete[] m_pModel->SubsetArray;
    delete m_pModel;
    // インスタンスを解放
    delete m_Instance;
}

EnemyFactory* EnemyFactory::GetInstance()
{
    // 初めて使うときにインスタンスを生成
    // それ以降は生成したインスタンスを渡す
    if (m_Instance == NULL)
    {
        m_Instance = new EnemyFactory();
        m_Instance->Init();
    }
    return m_Instance;
}

Enemy* EnemyFactory::GetEnemy()
{
    // リストから現在使われていない弾があるか探す
    auto iEnemy = std::find_if(m_EnemyList.begin(), m_EnemyList.end(),
        [](Enemy* pe) {return pe->GetActive() == false; });
    if (iEnemy != m_EnemyList.end())
    {	// 見つかったならそのオブジェクトをアクティブにして渡す
        (*iEnemy)->SetActive(true);
        return *iEnemy;
    }
    else
    {	// 見つからなかったならオブジェクトを生成
        Scene* scene = Manager::GetInstance()->GetScene();
        Enemy* pEnemy = scene->AddGameObject<Enemy>(1);
        // 弾リストに追加
        m_EnemyList.push_back(pEnemy);
        // 使用するモデルデータをセット
        pEnemy->Init(m_pModel);
        return pEnemy;
    }
}

void EnemyFactory::HideEnemy(Enemy* pEnemy)
{
    // 弾のアクティブフラグをOFF
    pEnemy->SetActive(false);
}
