#include <algorithm>
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "enemyFactory.h"
#include "enemy.h"
#include "model.h"


EnemyFactory::EnemyFactory()
{
}

EnemyFactory::~EnemyFactory()
{
	Uninit();
}

void EnemyFactory::Init()
{
	Model model;
	m_pModel = new MODEL();
	model.LoadObj("asset\\model\\enemyBox.obj", m_pModel);
}

Enemy * EnemyFactory::ActiveObject()
{
	// ���X�g���猻�ݎg���Ă��Ȃ��e�����邩�T��
    auto iEnemy = std::find_if(m_ObjectList.begin(), m_ObjectList.end(),
        [](Enemy* pe) {return pe->GetActive() == false; });
    if (iEnemy != m_ObjectList.end())
    {	// ���������Ȃ炻�̃I�u�W�F�N�g���A�N�e�B�u�ɂ��ēn��
        (*iEnemy)->SetActive(true);
        return *iEnemy;
    }
    else
    {	// ������Ȃ������Ȃ�I�u�W�F�N�g�𐶐�
        Scene* scene = Manager::GetInstance()->GetScene();
        Enemy* pEnemy = scene->AddGameObject<Enemy>(1);
        // �e���X�g�ɒǉ�
		m_ObjectList.push_back(pEnemy);
        // �g�p���郂�f���f�[�^���Z�b�g
        pEnemy->Init(m_pModel);
        return pEnemy;
    }
}

void EnemyFactory::HideObject(Enemy * enemy)
{
	// �e�̃A�N�e�B�u�t���O��OFF
    enemy->SetActive(false);
}

void EnemyFactory::Uninit()
{
	delete[] m_pModel->VertexArray;
	delete[] m_pModel->IndexArray;
	delete[] m_pModel->SubsetArray;
	delete m_pModel;
	Factory::Uninit();
}

void EnemyFactory::Clear()
{
    m_ObjectList.clear();
}
