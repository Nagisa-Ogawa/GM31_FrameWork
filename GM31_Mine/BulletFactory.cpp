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
	// ���X�g���猻�ݎg���Ă��Ȃ��e�����邩�T��
	auto iBullet = std::find_if(m_ObjectList.begin(), m_ObjectList.end(),
		[](Bullet* pb) {return pb->GetActive() == false; });
	if (iBullet != m_ObjectList.end())
	{	// ���������Ȃ炻�̃I�u�W�F�N�g���A�N�e�B�u�ɂ��ēn��
		(*iBullet)->SetActive(true);
		return *iBullet;
	}
	else
	{	// ������Ȃ������Ȃ�I�u�W�F�N�g�𐶐�
		Scene* scene = Manager::GetInstance()->GetScene();
		Bullet* pBullet = scene->AddGameObject<Bullet>(1);
		// �e���X�g�ɒǉ�
		m_ObjectList.push_back(pBullet);
		// �g�p���郂�f���f�[�^���Z�b�g
		pBullet->Init(m_pModel);
		return pBullet;
	}
}

void BulletFactory::HideObject(Bullet* bullet)
{
	// �e�̃A�N�e�B�u�t���O��OFF
   	bullet->SetActive(false);
}
