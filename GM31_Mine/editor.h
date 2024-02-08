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

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_editorObjectList)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_editorObjectList)
		);
	}

};
