#include "gameObject.h"
#include "MyImGuiManager.h"
#include "hierarchyGui.h"

void GameObject::SetDestroy()
{
	m_destroy = true;
	// 子供も削除
	for (auto child : m_transform->GetChildList()) {
		child->GetGameObject()->SetDestroy();
	}
}
bool GameObject::Destroy()
{
	if (m_destroy)
	{
		// 子供も削除
		//for (auto child : m_transform->GetChildList()) {
		//	child->GetGameObject()->SetDestroy();
		//	child->GetGameObject()->Destroy();
		//}
		// 親がいるなら子供リストから削除
		if (m_transform->GetParent()) {
			m_transform->SetParent(nullptr);
		}
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
	// 破棄フラグがONになっているコンポーネントは削除
	m_componentList.remove_if([](const auto& component)
	{return component->Destroy(); });
}


