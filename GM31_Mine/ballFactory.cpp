#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "ballFactory.h"
#include "Ball.h"
#include "model.h"

#define BOUND_CONST (1.0f)	// ���˕Ԃ�W��

BallFactory::BallFactory()
{
}

BallFactory::~BallFactory()
{
	Uninit();
}

void BallFactory::Init()
{
	srand(time(NULL));
	Model model;
	m_pModel = new MODEL();
	model.LoadObj("asset\\model\\ball.obj", m_pModel);
}


Ball* BallFactory::ActiveObject(std::string name)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(rand() % 50 - 25, 2.0f, rand() % 30 - 15);
	D3DXVECTOR2 vel = D3DXVECTOR2((rand() % 15 - 7) / 100.0f, (rand() % 15 - 7) / 100.0f);
	float mass = (float)(rand() % 5 + 1);
	// ���X�g���猻�ݎg���Ă��Ȃ��e�����邩�T��
	auto iBall = std::find_if(m_ObjectList.begin(), m_ObjectList.end(),
		[](Ball* pb) {return pb->GetActive() == false; });
	if (iBall != m_ObjectList.end())
	{	// ���������Ȃ炻�̃I�u�W�F�N�g���A�N�e�B�u�ɂ��ēn��
		(*iBall)->Init(m_pModel,pos,vel,mass);
		(*iBall)->SetActive(true);
		return *iBall;
	}
	else
	{	// ������Ȃ������Ȃ�I�u�W�F�N�g�𐶐�
		Scene* scene = Manager::GetInstance()->GetTitle();
		Ball* pball = scene->AddGameObject<Ball>(1, name);
		// �e���X�g�ɒǉ�
		m_ObjectList.push_back(pball);
		// �g�p���郂�f���f�[�^���Z�b�g
		pball->Init(m_pModel,pos,vel,mass);
		return pball;
	}
}

void BallFactory::HideObject(Ball* bullet)
{
	// �e�̃A�N�e�B�u�t���O��OFF
	bullet->SetActive(false);
}

void BallFactory::Uninit()
{
	Clear();
	delete[] m_pModel->VertexArray;
	delete[] m_pModel->IndexArray;
	delete[] m_pModel->SubsetArray;
	delete m_pModel;
	Factory::Uninit();
}

void BallFactory::Clear()
{
	m_ObjectList.clear();
}

void BallFactory::CollisionBallToBall()
{
	// �{�[�����m�̏Փ˂����m
	for (auto itr = m_ObjectList.begin(); itr != m_ObjectList.end(); itr++) {
		// �����ȊO�ł܂������蔻����s���Ă��Ȃ��{�[�����m�Ŕ�����s��
		for (auto oItr = itr; oItr != m_ObjectList.end(); oItr++) {
			if ((*itr) == (*oItr)) {
				// �����{�[�����m�Ȃ瓖����Ȃ�
				continue;
			}
			// �{�[���̓����蔻��
			D3DXVECTOR3 distance = (*itr)->GetTransform()->m_position - (*oItr)->GetTransform()->m_position;
			float length = D3DXVec3Length(&distance);
			float addRadius = (*itr)->GetTransform()->m_scale.x + (*oItr)->GetTransform()->m_scale.x;
			if (length <= addRadius) {
				auto aTrans = (*itr)->GetTransform();
				auto aBall = (*itr);
				auto bTrans = (*oItr)->GetTransform();
				auto bBall = (*oItr);

				D3DXVECTOR2 newAVel, newBVel;
				newAVel.x = ((aBall->GetMass() - BOUND_CONST * bBall->GetMass()) * aBall->GetVelocity().x + 
					(1 + BOUND_CONST) * bBall->GetMass() * bBall->GetVelocity().x) / (aBall->GetMass() + bBall->GetMass());
				newAVel.y = ((aBall->GetMass() - BOUND_CONST * bBall->GetMass()) * aBall->GetVelocity().y +
					(1 + BOUND_CONST) * bBall->GetMass() * bBall->GetVelocity().y) / (aBall->GetMass() + bBall->GetMass());

				newBVel.x = ((bBall->GetMass() - BOUND_CONST * aBall->GetMass()) * bBall->GetVelocity().x + 
					(1 + BOUND_CONST) * aBall->GetMass() * aBall->GetVelocity().x) / (aBall->GetMass() + bBall->GetMass());
				newBVel.y = ((bBall->GetMass() - BOUND_CONST * aBall->GetMass()) * bBall->GetVelocity().y +
					(1 + BOUND_CONST) * aBall->GetMass() * aBall->GetVelocity().y) / (aBall->GetMass() + bBall->GetMass());

				aBall->SetVelocity(newAVel);
				bBall->SetVelocity(newBVel);
			}
		}
	}
}
