#include "main.h"
#include "scene.h"
#include "script.h"

void Scene::Uninit()
{
	for (int i = 0; i < 3; i++)
	{
		for (GameObject* gameObject : m_GameObject[i])
		{
			gameObject->Uninit();
			delete gameObject;
		}
		m_GameObject[i].clear();
	}
}

void Scene::Update()
{
	for (int i = 0; i < 3; i++)
	{
		for (GameObject* gameObject : m_GameObject[i])
		{
			// �A�N�e�B�u�t���O��ON�Ȃ�X�V����������
			if (gameObject->GetActive())
				gameObject->Update();
		}
		m_GameObject[i].remove_if([](GameObject* object)
		{return object->Destroy(); });	// �����_��
	}
}

void Scene::Draw()
{
	for (int i = 0; i < 3; i++)
	{
		for (GameObject* gameObject : m_GameObject[i])
		{
			// �A�N�e�B�u�t���O��ON�Ȃ�`�悷��
			if (gameObject->GetActive())
				gameObject->Draw();
		}
	}
}


/// <summary>
/// �V�[���ɑ��݂���I�u�W�F�N�g�̌����擾����֐�
/// </summary>
/// <returns>�I�u�W�F�N�g�̌�</returns>
size_t Scene::GetGameObjectCount()
{
	size_t count = 0;
	for (int i = 0; i < 3; i++) {
		count += m_GameObject[i].size();
	}
	return count;
}


/// <summary>
/// �A�N�e�B�u�ȃI�u�W�F�N�g�̌����擾����֐�
/// </summary>
/// <returns>�I�u�W�F�N�g�̌�</returns>
int Scene::GetActiveGameObjectCount()
{
	int count = 0;
	for (int i = 0; i < 3; i++) {
		auto it = m_GameObject[i].begin();
		// ���ׂĂ̗v�f���������I���܂Ń��[�v
		while (true) {
			it = std::find_if(it, m_GameObject[i].end(), [](GameObject* obj) {return obj->GetActive(); });
			if (it == m_GameObject[i].end()) {
				break;
			}
			count++;
			it++;
		}
	}
	return count;
}


/// <summary>
/// ���ׂẴI�u�W�F�N�g�����X�g�Ŏ擾����֐�
/// </summary>
/// <returns>�I�u�W�F�N�g�̃��X�g</returns>
std::list<GameObject*> Scene::GetAllGameObjects()
{
	std::list<GameObject*> objList;
	for (int i = 0; i < 3; i++)
	{
		for (GameObject* gameObject : m_GameObject[i])
		{
			objList.push_back(gameObject);
		}
	}
	return objList;
}

/// <summary>
/// �V�[���̎��s���ɃX�N���v�g��Start�֐����Ăяo���֐�
/// </summary>
void Scene::CallScriptStartFunc()
{
	auto objectList = GetAllGameObjects();
	for (auto object : objectList) {
		Script* script = object->GetComponent<Script>();
		if (script == nullptr)	continue;
		script->Start();
	}
}
