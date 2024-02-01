#pragma once

#include <list>
#include <memory>
#include <vector>
#include <typeinfo>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/list.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/string.hpp"

#include "gameObject.h"

class Scene
{
protected:
	std::list<std::shared_ptr<GameObject>> m_sceneObjectList[3];		// �V�[���ɑ��݂���I�u�W�F�N�g�̃��X�g
	std::list<Transform*> m_parentObjectList;		// �V�[���ɒ��ڔz�u����Ă����ԏ�̐e��transform
	std::string m_name;		// �V�[���̖��O
	int m_registerID = 0;
public:
	virtual void Init();
	void Load();	// �V�[����ǂݍ��񂾍ۂɌĂ΂��֐�
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();


	std::string GetName() { return m_name; }
	GameObject* GetGameObjectWithID(int ID);
	GameObject* GetGameObjectWithName(std::string name);
	size_t GetGameObjectCount();	// �V�[���ɑ��݂���I�u�W�F�N�g�̌����擾����֐�
	int GetActiveGameObjectCount();	// �A�N�e�B�u�ȃI�u�W�F�N�g�̌����擾����֐�
	std::list<GameObject*> GetAllGameObjects();		// ���ׂẴI�u�W�F�N�g�����X�g�Ŏ擾����֐�

	void SetName(std::string name) { m_name = name; }

	void CallScriptStartFunc();		// �V�[���̎��s���ɃA�^�b�`����Ă���X�N���v�g��Start�֐����Ăяo���֐�
	void AddParentObject(Transform* transform) { m_parentObjectList.push_back(transform); }
	void DeleteParentObject(Transform* transform) { m_parentObjectList.remove(transform); }

	/// <summary>
	/// �I�u�W�F�N�g���V�[���ɒǉ�����֐�
	/// </summary>
	/// <typeparam name="T">�ǉ�����I�u�W�F�N�g�̃N���X��</typeparam>
	/// <param name="layer">�ǉ�����I�u�W�F�N�g�̃��C���[�i�`�揇�j</param>
	/// <param name="name">�ǉ�����I�u�W�F�N�g�̖��O</param>
	/// <returns>�ǉ������I�u�W�F�N�g�̃|�C���^</returns>
	template <typename T>
	T* AddGameObject(int layer,std::string name)
	{
		std::shared_ptr<GameObject> gameObject = std::make_shared<T>();
		m_sceneObjectList[layer].push_back(gameObject);
		// transform�R���|�[�l���g�͕K�{�Ȃ��߂�����AddComponent
		gameObject->SetTransform(gameObject->AddComponent<Transform>());
		// �I�u�W�F�N�g�̖��O��ݒ�
		gameObject->SetName(name);
		// ID��ݒ�
		gameObject->SetID(m_registerID);
		m_registerID++;
		// �ǉ������I�u�W�F�N�g�͐e�������Ȃ��̂ŃV�[���̃I�u�W�F�N�g�Ƃ��ēo�^
		m_parentObjectList.push_back(gameObject->GetTransform());
		gameObject->Init();

		return (T*)gameObject.get();
	}

	/// <summary>
	/// �I�u�W�F�N�g���擾����֐�
	/// </summary>
	/// <typeparam name="T">�擾����I�u�W�F�N�g��</typeparam>
	/// <returns>�擾�����I�u�W�F�N�g�̃|�C���^</returns>
	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (auto object : m_sceneObjectList[i])
			{
				if (typeid(*object) == typeid(T))// �^�𒲂ׂ�(RTTI���I�^���)
				{
					return (T*)object.get();
				}
			}
		}
		return nullptr;
	}

	/// <summary>
	/// �A�N�e�B�u�Ȏw�肳�ꂽ�I�u�W�F�N�g���擾����֐�
	/// </summary>
	/// <typeparam name="T">�擾����I�u�W�F�N�g�̃N���X��</typeparam>
	/// <returns>�擾�����I�u�W�F�N�g�̃|�C���^</returns>
	template <typename T>
	T* GetActiveGameObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (auto object : m_sceneObjectList[i])
			{
				if (object->GetActive() == false) {
					continue;
				}
				if (typeid(*object) == typeid(T))// �^�𒲂ׂ�(RTTI���I�^���)
				{
					return (T*)object.get();
				}
			}
		}
		return nullptr;
	}

	/// <summary>
	/// �����̃I�u�W�F�N�g���擾����֐�
	/// </summary>
	/// <typeparam name="T">�擾����I�u�W�F�N�g�̃N���X��</typeparam>
	/// <returns>�擾�����I�u�W�F�N�g�̃|�C���^</returns>
	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;
		for (int i = 0; i < 3; i++)
		{
			for (auto object : m_sceneObjectList[i])
			{
				if (typeid(*object) == typeid(T))// �^�𒲂ׂ�(RTTI���I�^���)
				{
					objects.push_back((T*)object.get());
				}
			}
		}
		return objects;
	}

	/// <summary>
	/// �A�N�e�B�u�ȕ����̃I�u�W�F�N�g���擾����֐�
	/// </summary>
	/// <typeparam name="T">�擾����I�u�W�F�N�g�̃N���X��</typeparam>
	/// <returns>�擾�����I�u�W�F�N�g�̃|�C���^</returns>
	template <typename T>
	std::vector<T*> GetActiveGameObjects()
	{
		std::vector<T*> objects;
		for (int i = 0; i < 3; i++)
		{
			for (auto object : m_sceneObjectList[i])
			{
				if (object->GetActive() == false) {
					continue;
				}
				if (typeid(*object) == typeid(T))// �^�𒲂ׂ�(RTTI���I�^���)
				{
					objects.push_back((T*)object.get());
				}
			}
		}
		return objects;
	}

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_sceneObjectList)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_sceneObjectList)
		);
	}

};
