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


//-----------------------------------------------------
// �Q�[���I�u�W�F�N�g�p�e�N���X
//------------------------------------------------------
// �G�f�B�^�p�I�u�W�F�N�g�����̃N���X���p�����܂�
//------------------------------------------------------
class GameObject
{
protected:
	Transform* m_transform = nullptr;	// �I�u�W�F�N�g�̃g�����X�t�H�[�����(���W�A��])
	std::list<std::unique_ptr<Component>> m_componentList;	// �I�u�W�F�N�g�̃R���|�[�l���g���X�g
	bool m_destroy = false;				// �I�u�W�F�N�g�̎��S�t���O
	bool m_active = true;				// �I�u�W�F�N�g�̕\���t���O
	std::string m_name{};				// �I�u�W�F�N�g�̖��O
	int m_id = -1;		// �I�u�W�F�N�g��ID
	int m_registerID = 0;	// �R���|�[�l���g�ɃZ�b�g����ID
	bool m_isGameObject = true;	// �Q�[���Ŏg�p�����I�u�W�F�N�g���ǂ����ifalse�̗�:�����蔻��\���p�I�u�W�F�N�g�Ȃ�

public:
	// Get�n�֐�
	Transform* GetTransform() { return m_transform; }
	bool GetActive() { return m_active; }
	std::string GetName() { return m_name; }
	int GetID() { return m_id; }
	bool GetIsGameObject() { return m_isGameObject; }
	// Set�n�֐�
	void SetTransform(Transform* transform) { m_transform = transform; }
	void SetDestroy();
	void SetActive(bool active) { m_active = active; }
	void SetName(std::string name) { m_name = name; }
	void SetID(int ID) { m_id = ID; }
	void SetIsGameObject(bool flag) { m_isGameObject = flag; }

	bool Destroy();

	template <typename T>
	T* AddComponent()
	{
		std::unique_ptr<Component> component = std::make_unique<T>();
		// �R���|�[�l���g���t���Ă���Q�[���I�u�W�F�N�g���i�[
		// ID���Z�b�g
		component->SetID(m_registerID);
		m_registerID++;
		// �R���|�[�l���g���̃I�u�W�F�N�g���Z�b�g
		component->SetGameObject(this);
		component->Init();
		T* TComponent = (T*)component.get();
		m_componentList.push_back(std::move(component));

		return TComponent;
	}

	template <typename T>
	T* GetComponent() 
	{
		for (const auto& pComp : m_componentList)
		{
			if (typeid(*pComp) == typeid(T))// �^�𒲂ׂ�(RTTI���I�^���)
			{
				return (T*)pComp.get();
			}
		}
		return nullptr;
	}

	std::list<Component*> GetAllComponent();

	virtual void Init();

	/// <summary>
	/// �I�u�W�F�N�g���[�h���ɌĂяo�����֐�
	/// </summary>
	virtual void Load();

	virtual void Uninit();

	virtual void Update();

	virtual void Draw();

	void CheckDestroyedComponent();

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_id),
			CEREAL_NVP(m_componentList),
			CEREAL_NVP(m_active),
			CEREAL_NVP(m_destroy),
			CEREAL_NVP(m_registerID)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_id),
			CEREAL_NVP(m_componentList),
			CEREAL_NVP(m_active),
			CEREAL_NVP(m_destroy),
			CEREAL_NVP(m_registerID)
		);
	}
};

