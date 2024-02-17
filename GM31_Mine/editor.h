#pragma once
#include <list>
#include <memory>
#include <typeinfo>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/list.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/string.hpp"

#include "gameObject.h"

class Editor {

private:
	std::list<std::unique_ptr<GameObject>> m_editorObjectList[3];		// �G�f�B�^�ɑ��݂���I�u�W�F�N�g�̃��X�g
	std::string m_name;		// �G�f�B�^�̖��O
	int m_registerID = 0;	// �G�f�B�^�I�u�W�F�N�g�ɃZ�b�g����ID
public:
	void Init();
	void Load();	// �G�f�B�^��ǂݍ��񂾍ۂɌĂ΂��֐�
	void Uninit();
	void Update();
	void Draw();

	std::string GetName() { return m_name; }
	void CheckDestroyedObject();


	/// <summary>
/// �G�f�B�^�p�I�u�W�F�N�g���G�f�B�^�ɒǉ�����֐�
/// </summary>
/// <typeparam name="T">�ǉ�����I�u�W�F�N�g�̃N���X��</typeparam>
/// <param name="layer">�ǉ�����I�u�W�F�N�g�̃��C���[�i�`�揇�j</param>
/// <param name="name">�ǉ�����I�u�W�F�N�g�̖��O</param>
/// <returns>�ǉ������I�u�W�F�N�g�̃|�C���^</returns>
	template <typename T>
	T* AddEditorObject(int layer, std::string name)
	{
		std::unique_ptr<GameObject> editorObject = std::make_unique<T>();
		// transform�R���|�[�l���g�͕K�{�Ȃ��߂�����AddComponent
		editorObject->SetTransform(editorObject->AddComponent<Transform>());
		// �I�u�W�F�N�g�̖��O��ݒ�
		editorObject->SetName(name);
		// ID��ݒ�
		editorObject->SetID(m_registerID);
		m_registerID++;
		// �G�f�B�^�p�I�u�W�F�N�g�ł��邱�Ƃ��Z�b�g
		editorObject->SetIsGameObject(false);
		editorObject->Init();
		T* TObject = (T*)editorObject.get();
		m_editorObjectList[layer].push_back(std::move(editorObject));

		return TObject;
	}

	/// <summary>
	/// �I�u�W�F�N�g���擾����֐�
	/// </summary>
	/// <typeparam name="T">�擾����I�u�W�F�N�g��</typeparam>
	/// <returns>�擾�����I�u�W�F�N�g�̃|�C���^</returns>
	template <typename T>
	T* GetEditorObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (const auto& object : m_editorObjectList[i])
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
	T* GetEditorObjectWithID(int ID)
	{
		// ID��-1�Ȃ疳��
		if (ID == -1) return nullptr;
		// �I�u�W�F�N�g�̃��X�g����ID�������I�u�W�F�N�g��T��
		for (int i = 0; i < 3; i++) {
			auto it = std::find_if(m_editorObjectList[i].begin(), m_editorObjectList[i].end(),
				[&ID](const auto& obj) {return obj->GetID() == ID; });
			if (it != m_editorObjectList[i].end()) {
				// ��v�����I�u�W�F�N�g���������Ȃ�Ԃ�
				return (T*)(it->get());
			}
		}
		// �Ȃ��Ȃ�null��Ԃ�
		return nullptr;
	}

	template <typename T>
	T* GetEditorObjectWithName(std::string name)
	{
		// ���O����Ȃ疳��
		if (name == "") return nullptr;
		// �I�u�W�F�N�g�̃��X�g����ID�������I�u�W�F�N�g��T��
		for (int i = 0; i < 3; i++) {
			auto it = std::find_if(m_editorObjectList[i].begin(), m_editorObjectList[i].end(),
				[&name](const auto& obj) {return obj->GetName() == name; });
			if (it != m_editorObjectList[i].end()) {
				// ��v�����I�u�W�F�N�g���������Ȃ�Ԃ�
				return (T*)(it->get());
			}
		}
		// �Ȃ��Ȃ�null��Ԃ�
		return nullptr;
	}


	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_editorObjectList),
			CEREAL_NVP(m_registerID)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_editorObjectList),
			CEREAL_NVP(m_registerID)
		);
	}

};
