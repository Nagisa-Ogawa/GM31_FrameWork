
#include <algorithm>
#include "manager.h"
#include "scene.h"
#include "BulletFactory.h"

BulletFactory::BulletFactory()
{
	// �Ȃɂ����Ȃ�
}

BulletFactory::BulletFactory(const BulletFactory & manager)
{
	// �C���X�^���X���R�s�[
	m_Instance = manager.m_Instance;
}

BulletFactory & BulletFactory::operator=(const BulletFactory & manager)
{
	// �C���X�^���X���R�s�[
	m_Instance = manager.m_Instance;
	return *m_Instance;
}

void BulletFactory::Init()
{
}

BulletFactory::~BulletFactory()
{
	// �C���X�^���X�����
	delete m_Instance;
}

BulletFactory * BulletFactory::GetInstance()
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

Bullet * BulletFactory::GetBullet()
{
	// ���X�g���猻�ݎg���Ă��Ȃ��e�����邩�T��
	auto bullet = std::find_if(m_BulletList.begin(), m_BulletList.end(), 
								[](Bullet* b) {return b->GetUse() == false; });
	if (bullet != m_BulletList.end())
	{	// ���������Ȃ炻�̃I�u�W�F�N�g��n��
		return *bullet;
	}
	else
	{	// ������Ȃ������Ȃ�I�u�W�F�N�g�𐶐�
		Scene* scene = Manager::GetInstance()->GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>(1);

	}
}
