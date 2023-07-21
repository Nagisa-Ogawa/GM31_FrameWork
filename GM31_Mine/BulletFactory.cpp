#include <algorithm>
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "bulletFactory.h"
#include "bullet.h"
#include "model.h"

BulletFactory::~BulletFactory()
{
}

void BulletFactory::Init()
{
	Model model;
	m_pModel = new MODEL();
	model.LoadObj("asset\\model\\torus.obj", m_pModel);
}

void BulletFactory::Uninit()
{
	delete[] m_pModel->VertexArray;
	delete[] m_pModel->IndexArray;
	delete[] m_pModel->SubsetArray;
	delete m_pModel;
}

Bullet* BulletFactory::ActiveObject()
{
	// リストから現在使われていない弾があるか探す
	auto iBullet = std::find_if(m_ObjectList.begin(), m_ObjectList.end(),
		[](Bullet* pb) {return pb->GetActive() == false; });
	if (iBullet != m_ObjectList.end())
	{	// 見つかったならそのオブジェクトをアクティブにして渡す
		(*iBullet)->SetActive(true);
		return *iBullet;
	}
	else
	{	// 見つからなかったならオブジェクトを生成
		Scene* scene = Manager::GetInstance()->GetScene();
		Bullet* pBullet = scene->AddGameObject<Bullet>(1);
		// 弾リストに追加
		m_ObjectList.push_back(pBullet);
		// 使用するモデルデータをセット
		pBullet->Init(m_pModel);
		return pBullet;
	}
}

void BulletFactory::HideObject(Bullet* bullet)
{
	// 弾のアクティブフラグをOFF
   	bullet->SetActive(false);
}
