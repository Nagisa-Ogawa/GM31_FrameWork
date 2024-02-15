#include "main.h"
#include "scene.h"
#include "script.h"

#include "renderer.h"
#include "cameraObject.h"
#include "player.h"
#include "box.h"
#include "meshField.h"
#include "sky.h"


void Scene::Init()
{
	m_editor = std::make_unique<Editor>();
	m_editor->Init();
	// ---------------------
	// �f�o�b�O�p
	// ---------------------
	AddGameObject<CameraObject>(0, "Camera");
	//auto player = AddGameObject<Player>(1, "Player");
	//auto box = AddGameObject<Box>(1, "box");
	//box->GetTransform()->m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 15.0f);
	//auto box1 = AddGameObject<Box>(1, "box1");
	//box1->GetTransform()->m_localPosition = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
	//box1->GetTransform()->SetParent(box->GetTransform());
	//auto meshField = AddGameObject<MeshField>(1, "Filed");

	AddGameObject<Sky>(1, "Sky");

}

void Scene::Load()
{
	for (int i = 0; i < 3; i++) {
		for (const auto& gameObject : m_sceneObjectList[i]){
			gameObject->Load();
		}
	}
	m_editor->Load();
}

void Scene::Uninit()
{
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_sceneObjectList[i])
		{
			gameObject->Uninit();
		}
		m_sceneObjectList[i].clear();
	}
}

void Scene::Update()
{
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_sceneObjectList[i])
		{
			// �A�N�e�B�u�t���O��ON�Ȃ�X�V����������
			if (gameObject->GetActive())
				gameObject->Update();
		}
	}
}

void Scene::Draw()
{
	// �`�������O�Ƀ��[�J���s�񂩂烏�[���h�s����쐬
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_sceneObjectList[i])
		{
			auto transform = gameObject->GetTransform();
			// �e�̂Ȃ��I�u�W�F�N�g�̃��[���h�s��쐬�֐����ĂсA�q�I�u�W�F�N�g�֘A��������
			if (transform->GetParent() == nullptr)
				transform->MakeWorldMatrix(nullptr);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_sceneObjectList[i])
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
		count += m_sceneObjectList[i].size();
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
		auto it = m_sceneObjectList[i].begin();
		// ���ׂĂ̗v�f���������I���܂Ń��[�v
		while (true) {
			it = std::find_if(it, m_sceneObjectList[i].end(), [](const auto& obj) {return obj->GetActive(); });
			if (it == m_sceneObjectList[i].end()) {
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
		for (const auto& gameObject : m_sceneObjectList[i])
		{
			objList.push_back(gameObject.get());
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

void Scene::CheckDestroyedObject()
{
	for (int i = 0; i < 3; i++)
	{
		// �I�u�W�F�N�g�̔j���t���O���`�F�b�N
		m_sceneObjectList[i].remove_if([](const auto& object)
		{return object->Destroy(); });
	}
	// �j������Ȃ��I�u�W�F�N�g�̃R���|�[�l���g�̔j���t���O���`�F�b�N
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_sceneObjectList[i]) {
			gameObject->CheckDestroyedComponent();
		}
	}
	m_editor->CheckDestroyedObject();
}


