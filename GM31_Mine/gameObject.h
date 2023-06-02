#pragma once

#include "component.h"
#include <list>

class GameObject
{
protected:

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	std::list<Component*> m_Component;

public:
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

	template <typename T>
	T* AddComponent()
	{
		Component* component = new T();
		m_Component.push_back(component);
		component->Init();

		return (T*)component;
	}

};
