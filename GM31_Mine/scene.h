#pragma once

#include <list>
#include <memory>
#include <typeinfo>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/list.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/string.hpp"

#include "editor.h"
#include "gameObject.h"

class Scene
{
private:
	std::list<std::unique_ptr<GameObject>> m_sceneObjectList[3];		// �V�[���ɑ��݂���I�u�W�F�N�g�̃��X�g
	std::unique_ptr<Editor> m_editor;
	std::string m_name;		// �V�[���̖��O
	int m_registerID = 0;	// �Q�[���I�u�W�F�N�g�ɃZ�b�g����ID
public:
	void Init();
	void Load();	// �V�[����ǂݍ��񂾍ۂɌĂ΂��֐�
	void Uninit();
	void Update();
	void Draw();


	std::string GetName() { return m_name; }
	Editor* GetEditor() { return m_editor.get(); }
	size_t GetGameObjectCount();	// �V�[���ɑ��݂���I�u�W�F�N�g�̌����擾����֐�
	int GetActiveGameObjectCount();	// �A�N�e�B�u�ȃI�u�W�F�N�g�̌����擾����֐�
	std::list<GameObject*> GetAllGameObjects();		// ���ׂẴI�u�W�F�N�g�����X�g�Ŏ擾����֐�

	void SetName(std::string name) { m_name = name; }

	void CallScriptStartFunc();		// �V�[���̎��s���ɃA�^�b�`����Ă���X�N���v�g��Start�֐����Ăяo���֐�
	void CheckDestroyedObject();

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
		std::unique_ptr<GameObject> gameObject = std::make_unique<T>();
		// transform�R���|�[�l���g�͕K�{�Ȃ��߂�����AddComponent
		gameObject->SetTransform(gameObject->AddComponent<Transform>());
		// �I�u�W�F�N�g�̖��O��ݒ�
		gameObject->SetName(name);
		// ID��ݒ�
		gameObject->SetID(m_registerID);
		m_registerID++;
		gameObject->Init();
		T* TObject = (T*)gameObject.get();
		m_sceneObjectList[layer].push_back(std::move(gameObject));

		return TObject;
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
			for (const auto& object : m_sceneObjectList[i])
			{
				if (typeid(*object) == typeid(T))// �^�𒲂ׂ�(RTTI���I�^���)
				{
					return (T*)object.get();
				}
			}
		}
		return nullptr;
	}

	template <typename T>
	T* GetGameObjectWithID(int ID) 
	{
		// ID��-1�Ȃ疳��
		if (ID == -1) return nullptr;
		// �I�u�W�F�N�g�̃��X�g����ID�������I�u�W�F�N�g��T��
		for (int i = 0; i < 3; i++) {
			auto it = std::find_if(m_sceneObjectList[i].begin(), m_sceneObjectList[i].end(),
				[&ID](const auto& obj) {return obj->GetID() == ID; });
			if (it != m_sceneObjectList[i].end()) {
				// ��v�����I�u�W�F�N�g���������Ȃ�Ԃ�
				return (T*)(it->get());
			}
		}
		// �Ȃ��Ȃ�null��Ԃ�
		return nullptr;
	}

	template <typename T>
	T* GetGameObjectWithName(std::string name) 
	{
		// ���O����Ȃ疳��
		if (name == "") return nullptr;
		// �I�u�W�F�N�g�̃��X�g����ID�������I�u�W�F�N�g��T��
		for (int i = 0; i < 3; i++) {
			auto it = std::find_if(m_sceneObjectList[i].begin(), m_sceneObjectList[i].end(),
				[&name](const auto& obj) {return obj->GetName() == name; });
			if (it != m_sceneObjectList[i].end()) {
				// ��v�����I�u�W�F�N�g���������Ȃ�Ԃ�
				return (T*)(it->get());
			}
		}
		// �Ȃ��Ȃ�null��Ԃ�
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
			for (const auto& object : m_sceneObjectList[i])
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
			for (const auto& object : m_sceneObjectList[i])
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
			for (const auto& object : m_sceneObjectList[i])
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
