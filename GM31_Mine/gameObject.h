#pragma once

#include <list>
#include <string>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/list.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/polymorphic.hpp"

#include "component.h"
#include "transform.h"

class GameObject
{
protected:
	Transform* m_transform = nullptr;	// �I�u�W�F�N�g�̃g�����X�t�H�[�����(���W�A��])
	std::list<std::shared_ptr<Component>> m_componentList;	// �I�u�W�F�N�g�̃R���|�[�l���g���X�g
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

	template <typename T>
	T* AddComponent()
	{
		std::shared_ptr<Component> component = std::make_shared<T>();
		// �R���|�[�l���g���t���Ă���Q�[���I�u�W�F�N�g���i�[
		m_componentList.push_back(component);
		component->SetGameObject(this);
		component->Init();

		return (T*)component.get();
	}

	template <typename T>
	T* GetComponent() 
	{
		for (auto pComp : m_componentList)
		{
			if (typeid(*pComp) == typeid(T))// �^�𒲂ׂ�(RTTI���I�^���)
			{
				return (T*)pComp.get();
			}
		}
		return nullptr;
	}

	std::list<Component*> GetAllComponent()
	{
		std::list<Component*> list;
		for (auto component : m_componentList) {
			list.push_back(component.get());
		}
		return list;
	}

	virtual void Init() 
	{
		for (auto component : m_componentList) {
			component->Init();
		}
	}
	virtual void Uninit()
	{
		for (auto component : m_componentList) {
			component->Uninit();
		}
		m_componentList.clear();
	}
	virtual void Update()
	{
		for (auto component : m_componentList) {
			component->Update();
		}
	}
	virtual void Draw()
	{
		for (auto component : m_componentList) {
			component->Draw();
		}
	}

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_active),
			CEREAL_NVP(m_destroy),
			CEREAL_NVP(m_componentList)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_active),
			CEREAL_NVP(m_destroy),
			CEREAL_NVP(m_componentList)
		);
	}

};
