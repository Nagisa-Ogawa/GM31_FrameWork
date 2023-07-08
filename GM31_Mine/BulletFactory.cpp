
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
	// �Ȃɂ����Ȃ�
}

BulletFactory::BulletFactory(const BulletFactory& manager)
{
	// �C���X�^���X���R�s�[
	m_Instance = manager.m_Instance;
}

BulletFactory& BulletFactory::operator=(const BulletFactory& manager)
{
	// �C���X�^���X���R�s�[
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
	// �C���X�^���X�����
	delete m_Instance;
}


BulletFactory* BulletFactory::GetInstance()
{
	// ���߂Ďg���Ƃ��ɃC���X�^���X�𐶐�
	// ����ȍ~�͐��������C���X�^���X��n��
	if (m_Instance == NULL)
	{
		m_Instance = new BulletFactory();
		m_Instance->Init();
	}
	return m_Instance;

}

Bullet* BulletFactory::GetBullet()
{
	// ���X�g���猻�ݎg���Ă��Ȃ��e�����邩�T��
	auto iBullet = std::find_if(m_BulletList.begin(), m_BulletList.end(),
		[](Bullet* pb) {return pb->GetActive() == false; });
	if (iBullet != m_BulletList.end())
	{	// ���������Ȃ炻�̃I�u�W�F�N�g���A�N�e�B�u�ɂ��ēn��
		(*iBullet)->SetActive(true);
		return *iBullet;
	}
	else
	{	// ������Ȃ������Ȃ�I�u�W�F�N�g�𐶐�
		Scene* scene = Manager::GetInstance()->GetScene();
		Bullet* pBullet = scene->AddGameObject<Bullet>(1);
		// �e���X�g�ɒǉ�
		m_BulletList.push_back(pBullet);
		// �g�p���郂�f���f�[�^���Z�b�g
		pBullet->Init(m_pModel);
		return pBullet;
	}
}

void BulletFactory::HideBullet(Bullet* pBullet)
{
	// �e�̃A�N�e�B�u�t���O��OFF
	pBullet->SetActive(false);
}

