
#include <algorithm>
#include "manager.h"
#include "scene.h"
#include "model.h"
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

BulletFactory::~BulletFactory()
{
	// �C���X�^���X�����
	delete m_Instance;
}

void BulletFactory::Init()
{
	Model model;
	model.LoadObj("asset\\model\\torus.obj", m_pModel);
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

Bullet* BulletFactory::GetBullet()
{
	// ���X�g���猻�ݎg���Ă��Ȃ��e�����邩�T��
	auto bullet = std::find_if(m_BulletList.begin(), m_BulletList.end(), 
								[](Bullet* b) {return b->GetActive() == false; });
	if (bullet != m_BulletList.end())
	{	// ���������Ȃ炻�̃I�u�W�F�N�g���A�N�e�B�u�ɂ��ēn��
		(*bullet)->SetActive(true);
		return *bullet;
	}
	else
	{	// ������Ȃ������Ȃ�I�u�W�F�N�g�𐶐�
		Scene* scene = Manager::GetInstance()->GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>(1);
		// �e���X�g�ɒǉ�
		m_BulletList.push_back(bullet);
		// �g�p���郂�f���f�[�^���Z�b�g
		bullet->SetModel(m_pModel);
	}
}

