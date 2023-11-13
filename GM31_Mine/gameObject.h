#pragma once

#include <list>

#include "component.h"
#include "transform.h"

class GameObject
{
protected:
	Transform* m_transform = nullptr;
	std::list<Component*> m_component;
	bool m_destroy = false;
	bool m_active = true;

public:
	void SetTransform(Transform* transform) { m_transform = transform; }
	Transform* GetTransform() { return m_transform; }
	void SetDestroy() { m_destroy = true; }
	bool GetActive() { return m_active; }
	void SetActive(bool active) { m_active = active; }

	bool Destroy()
	{
		if (m_destroy)
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
		m_component.push_back(component);
		component->SetGameObject(this);
		component->Init();

		return (TFactory*)component;
	}

	template <typename T>
	T* GetComponent() 
	{
		for (Component* pComp : m_component)
		{
			if (typeid(*pComp) == typeid(T))// 型を調べる(RTTI動的型情報)
			{
				return (T*)pComp;
			}
		}
		return nullptr;
	}

	virtual void Init() 
	{
		for (Component* component : m_component) {
			component->Init();
		}
	}
	virtual void Uninit()
	{
		for (Component* component : m_component) {
			component->Uninit();
			delete component;
		}
		m_component.clear();
	}
	virtual void Update()
	{
		for (Component* component : m_component) {
			component->Update();
		}
	}
	virtual void Draw()
	{
		for (Component* component : m_component) {
			component->Draw();
		}
	}
};
