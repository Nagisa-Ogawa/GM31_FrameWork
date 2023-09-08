#include <algorithm>
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "enemyFactory.h"
#include "enemy.h"
#include "model.h"


EnemyFactory::EnemyFactory()
{
}

EnemyFactory::~EnemyFactory()
{
	Uninit();
}

void EnemyFactory::Init()
{
	Model model;
	m_pModel = new MODEL();
	model.LoadObj("asset\\model\\enemyBox.obj", m_pModel);
}

Enemy * EnemyFactory::ActiveObject()
{
	// リストから現在使われていない弾があるか探す
    auto iEnemy = std::find_if(m_ObjectList.begin(), m_ObjectList.end(),
        [](Enemy* pe) {return pe->GetActive() == false; });
    if (iEnemy != m_ObjectList.end())
    {	// 見つかったならそのオブジェクトをアクティブにして渡す
        (*iEnemy)->SetActive(true);
        return *iEnemy;
    }
    else
    {	// 見つからなかったならオブジェクトを生成
        Scene* scene = Manager::GetInstance()->GetScene();
        Enemy* pEnemy = scene->AddGameObject<Enemy>(1);
        // 弾リストに追加
		m_ObjectList.push_back(pEnemy);
        // 使用するモデルデータをセット
        pEnemy->Init(m_pModel);
        return pEnemy;
    }
}

void EnemyFactory::HideObject(Enemy * enemy)
{
	// 弾のアクティブフラグをOFF
    enemy->SetActive(false);
}

void EnemyFactory::Uninit()
{
	delete[] m_pModel->VertexArray;
	delete[] m_pModel->IndexArray;
	delete[] m_pModel->SubsetArray;
	delete m_pModel;
	Factory::Uninit();
}

void EnemyFactory::Clear()
{
    m_ObjectList.clear();
}
