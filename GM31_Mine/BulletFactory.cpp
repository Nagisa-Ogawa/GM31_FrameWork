
#include <algorithm>
#include "manager.h"
#include "scene.h"
#include "model.h"
#include "BulletFactory.h"

BulletFactory::BulletFactory()
{
	// なにもしない
}

BulletFactory::BulletFactory(const BulletFactory & manager)
{
	// インスタンスをコピー
	m_Instance = manager.m_Instance;
}

BulletFactory & BulletFactory::operator=(const BulletFactory & manager)
{
	// インスタンスをコピー
	m_Instance = manager.m_Instance;
	return *m_Instance;
}

BulletFactory::~BulletFactory()
{
	// インスタンスを解放
	delete m_Instance;
}

void BulletFactory::Init()
{
	Model model;
	model.LoadObj("asset\\model\\torus.obj", m_pModel);
}

BulletFactory * BulletFactory::GetInstance()
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
	auto bullet = std::find_if(m_BulletList.begin(), m_BulletList.end(), 
								[](Bullet* b) {return b->GetActive() == false; });
	if (bullet != m_BulletList.end())
	{	// 見つかったならそのオブジェクトをアクティブにして渡す
		(*bullet)->SetActive(true);
		return *bullet;
	}
	else
	{	// 見つからなかったならオブジェクトを生成
		Scene* scene = Manager::GetInstance()->GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>(1);
		// 弾リストに追加
		m_BulletList.push_back(bullet);
		// 使用するモデルデータをセット
		bullet->SetModel(m_pModel);
	}
}

