
#include <algorithm>
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "bulletFactory.h"
#include "bullet.h"
#include "model.h"

BulletFactory* BulletFactory::m_Instance = NULL;

BulletFactory::BulletFactory()
{
	// なにもしない
}

BulletFactory::BulletFactory(const BulletFactory& manager)
{
	// インスタンスをコピー
	m_Instance = manager.m_Instance;
}

BulletFactory& BulletFactory::operator=(const BulletFactory& manager)
{
	// インスタンスをコピー
	m_Instance = manager.m_Instance;
	return *m_Instance;
}

void BulletFactory::Init()
{
	Model model;
	m_pModel = new MODEL();
	model.LoadObj("asset\\model\\torus.obj", m_pModel);
}

BulletFactory::~BulletFactory()
{
	delete[] m_pModel->VertexArray;
	delete[] m_pModel->IndexArray;
	delete[] m_pModel->SubsetArray;
	delete m_pModel;
	// インスタンスを解放
	delete m_Instance;
}


BulletFactory* BulletFactory::GetInstance()
{
	// 初めて使うときにインスタンスを生成
	// それ以降は生成したインスタンスを渡す
	if (m_Instance == NULL)
	{
		m_Instance = new BulletFactory();
		m_Instance->Init();
	}
	return m_Instance;

}

Bullet* BulletFactory::GetBullet()
{
	// リストから現在使われていない弾があるか探す
	auto iBullet = std::find_if(m_BulletList.begin(), m_BulletList.end(),
		[](Bullet* pb) {return pb->GetActive() == false; });
	if (iBullet != m_BulletList.end())
	{	// 見つかったならそのオブジェクトをアクティブにして渡す
		(*iBullet)->SetActive(true);
		return *iBullet;
	}
	else
	{	// 見つからなかったならオブジェクトを生成
		Scene* scene = Manager::GetInstance()->GetScene();
		Bullet* pBullet = scene->AddGameObject<Bullet>(1);
		// 弾リストに追加
		m_BulletList.push_back(pBullet);
		// 使用するモデルデータをセット
		pBullet->Init(m_pModel);
		return pBullet;
	}
}

void BulletFactory::HideBullet(Bullet* pBullet)
{
	// 弾のアクティブフラグをOFF
	pBullet->SetActive(false);
}

