#pragma once

#include <list>

#include "component.h"
#include "transform.h"

class GameObject
{
protected:
	Transform* m_Transform = nullptr;
	std::list<Component*> m_Component;
	bool m_Destroy = false;
	bool m_Active = true;

public:
	void SetTransform(Transform* transform) { m_Transform = transform; }
	Transform* GetTransform() { return m_Transform; }
	void SetDestroy() { m_Destroy = true; }
	bool GetActive() { return m_Active; }
	void SetActive(bool active) { m_Active = active; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename TFactory>
	TFactory* AddComponent()
	{
		Component* component = new TFactory();
		// コンポーネントが付いているゲームオブジェクトを格納
		m_Component.push_back(component);
		component->SetGameObject(this);
		component->Init();

		return (TFactory*)component;
	}

	virtual void Init() 
	{
		for (Component* component : m_Component) {
			component->Init();
		}
	}
	virtual void Uninit()
	{
		for (Component* component : m_Component) {
			component->Uninit();
			delete component;
		}
		m_Component.clear();
	}
	virtual void Update()
	{
		for (Component* component : m_Component) {
			component->Update();
		}
	}
	virtual void Draw()
	{
		for (Component* component : m_Component) {
			component->Draw();
		}
	}


};
