#include <algorithm>
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "enemyFactory.h"
#include "enemy.h"
#include "model.h"

EnemyFactory* EnemyFactory::m_Instance = NULL;

EnemyFactory::EnemyFactory()
{
    // �Ȃɂ����Ȃ�
}

EnemyFactory::EnemyFactory(const EnemyFactory& manager)
{
    // �C���X�^���X���R�s�[
    m_Instance = manager.m_Instance;
}

EnemyFactory& EnemyFactory::operator=(const EnemyFactory& manager)
{
    // �C���X�^���X���R�s�[
    m_Instance = manager.m_Instance;
    return *m_Instance;
}

void EnemyFactory::Init()
{
    Model model;
    m_pModel = new MODEL();
    model.LoadObj("asset\\model\\box.obj", m_pModel);
}

EnemyFactory::~EnemyFactory()
{
    delete[] m_pModel->VertexArray;
    delete[] m_pModel->IndexArray;
    delete[] m_pModel->SubsetArray;
    delete m_pModel;
    // �C���X�^���X�����
    delete m_Instance;
}

EnemyFactory* EnemyFactory::GetInstance()
{
    // ���߂Ďg���Ƃ��ɃC���X�^���X�𐶐�
    // ����ȍ~�͐��������C���X�^���X��n��
    if (m_Instance == NULL)
    {
        m_Instance = new EnemyFactory();
        m_Instance->Init();
    }
    return m_Instance;
}

Enemy* EnemyFactory::GetEnemy()
{
    // ���X�g���猻�ݎg���Ă��Ȃ��e�����邩�T��
    auto iEnemy = std::find_if(m_EnemyList.begin(), m_EnemyList.end(),
        [](Enemy* pe) {return pe->GetActive() == false; });
    if (iEnemy != m_EnemyList.end())
    {	// ���������Ȃ炻�̃I�u�W�F�N�g���A�N�e�B�u�ɂ��ēn��
        (*iEnemy)->SetActive(true);
        return *iEnemy;
    }
    else
    {	// ������Ȃ������Ȃ�I�u�W�F�N�g�𐶐�
        Scene* scene = Manager::GetInstance()->GetScene();
        Enemy* pEnemy = scene->AddGameObject<Enemy>(1);
        // �e���X�g�ɒǉ�
        m_EnemyList.push_back(pEnemy);
        // �g�p���郂�f���f�[�^���Z�b�g
        pEnemy->Init(m_pModel);
        return pEnemy;
    }
}

void EnemyFactory::HideEnemy(Enemy* pEnemy)
{
    // �e�̃A�N�e�B�u�t���O��OFF
    pEnemy->SetActive(false);
}
