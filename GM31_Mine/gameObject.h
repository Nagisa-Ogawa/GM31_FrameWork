#pragma once

#include <list>
#include <string>

#include "component.h"
#include "transform.h"

class GameObject
{
protected:
	Transform* m_transform = nullptr;	// �I�u�W�F�N�g�̃g�����X�t�H�[�����(���W�A��])
	std::list<Component*> m_component;	// �I�u�W�F�N�g�̃R���|�[�l���g���X�g
	bool m_destroy = false;				// �I�u�W�F�N�g�̎��S�t���O
	bool m_active = true;				// �I�u�W�F�N�g�̕\���t���O
	std::string m_name{};				// �I�u�W�F�N�g�̖��O

public:
	// Get�n�֐�
	Transform* GetTransform() { return m_transform; }
	bool GetActive() { return m_active; }
	std::string GetName() { return m_name; }
	// Set�n�֐�
	void SetTransform(Transform* transform) { m_transform = transform; }
	void SetDestroy() { m_destroy = true; }
	void SetActive(bool active) { m_active = active; }
	void SetName(std::string name) { m_name = name; }

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
		// �R���|�[�l���g���t���Ă���Q�[���I�u�W�F�N�g���i�[
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
			if (typeid(*pComp) == typeid(T))// �^�𒲂ׂ�(RTTI���I�^���)
			{
				return (T*)pComp;
			}
		}
		return nullptr;
	}

	std::list<Component*> GetAllComponent()
	{
		return m_component;
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
