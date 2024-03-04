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

//-----------------------------------------------------
// �V�[���p�N���X
//------------------------------------------------------
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

	// Get�n�֐�
	std::string GetName() { return m_name; }
	Editor* GetEditor() { return m_editor.get(); }
	// Set�n�֐�
	void SetName(std::string name) { m_name = name; }

	// GetGameObject�n�֐�
	GameObject* GetGameObjectWithID(int ID);				// ID����Q�[���I�u�W�F�N�g���擾����֐�
	GameObject* GetGameObjectWithName(std::string name);	// ���O����Q�[���I�u�W�F�N�g���擾����֐�
	std::list<GameObject*> GetAllGameObjects();		// ���ׂẴI�u�W�F�N�g�����X�g�Ŏ擾����֐�
	std::list<GameObject*> GetMostParentObjects();	// �e�̂��Ȃ��I�u�W�F�N�g�����X�g�Ŏ擾����֐�

	void CreateObjectNode(GameObject* object);				// �I�u�W�F�N�g���쐬�������Ƀq�G�����L�[�E�B���h�E�̃m�[�h���쐬����֐�
	void CallScriptStartFunc();				// �V�[���̎��s���ɃA�^�b�`����Ă���X�N���v�g��Start�֐����Ăяo���֐�
	void CheckSameName(std::string& name);	// �������O�̃I�u�W�F�N�g�����邩�ǂ������ׁA�������O���������Ȃ�V�������O������֐�
	void CheckDestroyedObject();			// �V�[���̃I�u�W�F�N�g���j������Ă��邩���`�F�b�N����֐�

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
		// ���O���g�p����Ă��Ȃ����`�F�b�N
		CheckSameName(name);
		// �I�u�W�F�N�g�̖��O��ݒ�
		gameObject->SetName(name);
		// ID��ݒ�
		gameObject->SetID(m_registerID);
		m_registerID++;
		// �Q�[���I�u�W�F�N�g�ł��邱�Ƃ��Z�b�g
		gameObject->SetIsGameObject(true);
		gameObject->Init();
		T* TObject = (T*)gameObject.get();
		// ���݂̃V�[���ɃI�u�W�F�N�g��ǉ����Ă���Ȃ�q�G�����L�[�E�B���h�E�ɂ��ǉ�
		CreateObjectNode(gameObject.get());
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

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_sceneObjectList),
			CEREAL_NVP(m_editor),
			CEREAL_NVP(m_registerID)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_sceneObjectList),
			CEREAL_NVP(m_editor),
			CEREAL_NVP(m_registerID)
		);
	}

	void CollsionTest();
};
