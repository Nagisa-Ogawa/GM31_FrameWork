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
	// �e�N�X�`���̓ǂݍ���
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
	// ���X�g���猻�ݎg���Ă��Ȃ��e�����邩�T��
	auto iTree = std::find_if(m_ObjectList.begin(), m_ObjectList.end(),
		[](Tree* pb) {return pb->GetActive() == false; });
	if (iTree != m_ObjectList.end())
	{	// ���������Ȃ炻�̃I�u�W�F�N�g���A�N�e�B�u�ɂ��ēn��
		(*iTree)->Init(m_Texture);
		(*iTree)->SetActive(true);
		return *iTree;
	}
	else
	{	// ������Ȃ������Ȃ�I�u�W�F�N�g�𐶐�
		Scene* scene = Manager::GetInstance()->GetScene();
		Tree* pTree = scene->AddGameObject<Tree>(1);
		// �e���X�g�ɒǉ�
		m_ObjectList.push_back(pTree);
		// �g�p���郂�f���f�[�^���Z�b�g
		pTree->Init(m_Texture);
		return pTree;
	}
}

void TreeFactory::HideObject(Tree* bullet)
{
	// �e�̃A�N�e�B�u�t���O��OFF
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
