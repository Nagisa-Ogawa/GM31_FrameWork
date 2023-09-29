#include <algorithm>
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "treeFactory.h"
#include "tree.h"

TreeFactory::TreeFactory()
{
}

TreeFactory::~TreeFactory()
{
	Uninit();
}

void TreeFactory::Init()
{
	// テクスチャの読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/tree.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);
}


Tree* TreeFactory::ActiveObject()
{
	// リストから現在使われていない弾があるか探す
	auto iTree = std::find_if(m_ObjectList.begin(), m_ObjectList.end(),
		[](Tree* pb) {return pb->GetActive() == false; });
	if (iTree != m_ObjectList.end())
	{	// 見つかったならそのオブジェクトをアクティブにして渡す
		(*iTree)->Init(m_Texture);
		(*iTree)->SetActive(true);
		return *iTree;
	}
	else
	{	// 見つからなかったならオブジェクトを生成
		Scene* scene = Manager::GetInstance()->GetScene();
		Tree* pTree = scene->AddGameObject<Tree>(1);
		// 弾リストに追加
		m_ObjectList.push_back(pTree);
		// 使用するモデルデータをセット
		pTree->Init(m_Texture);
		return pTree;
	}
}

void TreeFactory::HideObject(Tree* bullet)
{
	// 弾のアクティブフラグをOFF
	bullet->SetActive(false);
}

void TreeFactory::Uninit()
{
	m_Texture->Release();
	delete m_Texture;
	Factory::Uninit();
}

void TreeFactory::Clear()
{
	m_ObjectList.clear();
}
