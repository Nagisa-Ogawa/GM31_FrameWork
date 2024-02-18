#include "gameObject.h"
#include "myImGuiManager.h"
#include "hierarchyGui.h"

void GameObject::SetDestroy()
{
	m_destroy = true;
	// �q�����폜
	for (auto child : m_transform->GetChildList()) {
		child->GetGameObject()->SetDestroy();
	}
	// �e������Ȃ�q�����X�g����폜
	if (m_transform->GetParent()) {
		m_transform->SetParent(nullptr);
	}
}
bool GameObject::Destroy()
{
	if (m_destroy)
	{
		if (m_isGameObject) {
			auto hierarchy = MyImGuiManager::GetInstance()->GetImGui<HierarchyGui>();
			hierarchy->DeleteObjectNode(m_id);
		}
		Uninit();
		return true;
	}
	else
	{
		return false;
	}
	
}

std::list<Component*> GameObject::GetAllComponent()
{
	std::list<Component*> list;
	for (const auto& component : m_componentList) {
		list.push_back(component.get());
	}
	return list;

}

void GameObject::Init()
{
	{
		for (const auto& component : m_componentList) {
			component->Init();
		}
	}
}

void GameObject::Load()
{
	m_transform = GetComponent<Transform>();
	for (const auto& component : m_componentList) {
		component->SetGameObject(this);
		component->Load();
	}
}

void GameObject::Uninit()
{
	for (const auto& component : m_componentList) {
		component->Uninit();
	}
	m_componentList.clear();
}

void GameObject::Update()
{
	for (const auto& component : m_componentList) {
		component->Update();
	}
}

void GameObject::Draw()
{
	for (const auto& component : m_componentList) {
		component->Draw();
	}
}

void GameObject::CheckDestroyedComponent()
{
	// �j���t���O��ON�ɂȂ��Ă���R���|�[�l���g�͍폜
	m_componentList.remove_if([](const auto& component)
	{return component->Destroy(); });
}


