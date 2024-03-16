#include <algorithm>
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "luaManager.h"
#include "myImGuiManager.h"
#include "hierarchyGui.h"
#include "script.h"
#include "renderer.h"
#include "cameraObject.h"
#include "sky.h"
#include "CollisionManager.h"
#include "boxCollision.h"

void Scene::Init()
{
	m_editor = std::make_unique<Editor>();
	m_editor->Init();
	AddGameObject<CameraObject>(0, "MainCamera");
	AddGameObject<Sky>(1, "Sky");

	// �q�G�����L�[�E�B���h�E�̃I�u�W�F�N�g�؍\����������
	auto hierarchy = MyImGuiManager::GetInstance()->GetImGui<HierarchyGui>();
	hierarchy->InitObjectTree();
}

void Scene::Load()
{
	// �V�[���ɂ���I�u�W�F�N�g��Load�֐����Ăяo��
	for (int i = 0; i < 3; i++) {
		for (const auto& gameObject : m_sceneObjectList[i]){
			gameObject->Load();
		}
	}
	// �Q�[���I�u�W�F�N�g���S�ă��[�h���ꂽ�Ȃ�e�I�u�W�F�N�g���Z�b�g
	for (int i = 0; i < 3; i++) {
		for (const auto& gameObject : m_sceneObjectList[i]) {
			gameObject->GetTransform()->LoadParent();
		}
	}
	// �G�f�B�^�p�V�[����Load�֐����Ăяo��
	m_editor->Load();
	// �q�G�����L�[�E�B���h�E�̃I�u�W�F�N�g�؍\����������
	auto hierarchy = MyImGuiManager::GetInstance()->GetImGui<HierarchyGui>();
	hierarchy->InitObjectTree();
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
	m_editor->Uninit();
	LuaManager::GetInstance()->ClearScriptList();
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
	// CollsionTest();
}


void Scene::CollsionTest() {
	auto player = GetGameObjectWithName("Humanoid");
	auto enemy = GetGameObjectWithName("Enemy");
	auto playerColl = player->GetComponent<BoxCollision>();
	auto enemyColl = enemy->GetComponent<BoxCollision>();
	float l = -1;
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (CollisionManager::GetInstance()->Collision_BoxToBox(playerColl, enemyColl, &l, &vec)) {
		
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
/// ID����Q�[���I�u�W�F�N�g���擾����֐�
/// </summary>
/// <param name="ID"></param>
/// <returns></returns>
GameObject* Scene::GetGameObjectWithID(int ID)
{
	// ID��-1�Ȃ疳��
	if (ID == -1) return nullptr;
	// �I�u�W�F�N�g�̃��X�g����ID�������I�u�W�F�N�g��T��
	for (int i = 0; i < 3; i++) {
		auto it = std::find_if(m_sceneObjectList[i].begin(), m_sceneObjectList[i].end(),
			[&ID](const auto& obj) {return obj->GetID() == ID; });
		if (it != m_sceneObjectList[i].end()) {
			// ��v�����I�u�W�F�N�g���������Ȃ�Ԃ�
			return (it->get());
		}
	}
	// �Ȃ��Ȃ�null��Ԃ�
	return nullptr;
}


/// <summary>
/// ���O����Q�[���I�u�W�F�N�g���擾����֐�
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
GameObject* Scene::GetGameObjectWithName(std::string name)
{
	// ���O����Ȃ疳��
	if (name == "") return nullptr;
	// �I�u�W�F�N�g�̃��X�g����ID�������I�u�W�F�N�g��T��
	for (int i = 0; i < 3; i++) {
		auto it = std::find_if(m_sceneObjectList[i].begin(), m_sceneObjectList[i].end(),
			[&name](const auto& obj) {return obj->GetName() == name; });
		if (it != m_sceneObjectList[i].end()) {
			// ��v�����I�u�W�F�N�g���������Ȃ�Ԃ�
			return it->get();
		}
	}
	// �Ȃ��Ȃ�null��Ԃ�
	return nullptr;
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
/// �e�����Ȃ��i��Ԑe�j�I�u�W�F�N�g�����X�g�ɂ��Ď擾����֐�
/// </summary>
/// <returns></returns>
std::list<GameObject*> Scene::GetMostParentObjects()
{
	std::list<GameObject*> objList;
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_sceneObjectList[i])
		{
			// �e�����Ȃ��Ȃ�i��Ԑe�Ȃ�j
			if (gameObject->GetTransform()->GetParent() == nullptr) {
				objList.push_back(gameObject.get());
			}
		}
	}
	return objList;
}


void Scene::CreateObjectNode(GameObject* object)
{
	if (Manager::GetInstance()->GetScene() == this) {
		auto hierarchy = MyImGuiManager::GetInstance()->GetImGui<HierarchyGui>();
		hierarchy->AddObjectNode(object);
	}
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

/// <summary>
/// �������O�̃I�u�W�F�N�g�����邩�ǂ������ׁA�������O���������Ȃ�V�������O������֐�
/// </summary>
/// <param name="name">�I�u�W�F�N�g�̖��O</param>
void Scene::CheckSameName(std::string& name)
{
	auto objectList = GetAllGameObjects();
	// �������O�̃I�u�W�F�N�g�Ȃ����O�ɂȂ�܂Ń��[�v
	std::string newName = name;
	int count = 0;
	while (true) {
		// �������O�̃I�u�W�F�N�g�����邩�T��
		auto it = std::find_if(objectList.begin(), objectList.end(), 
			[&newName](GameObject* object) { return object->GetName() == newName; });
		if (it != objectList.end()) {
			count++;
			// �������O���������Ȃ疼�O��ς���
			newName = name + "(" + std::to_string(count) + ")";
		}
		else {
			// �������O�̃I�u�W�F�N�g�͂Ȃ��̂ŏI��
			name = newName;
			break;
		}
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


