#include "main.h"
#include "editor.h"
#include "renderer.h"
#include "manager.h"

#include "editorCameraObject.h"

void Editor::Init()
{
	AddEditorObject<EditorCameraObject>(0, "EditorCamera");
}

void Editor::Load()
{
	for (int i = 0; i < 3; i++) {
		for (const auto& gameObject : m_editorObjectList[i]) {
			gameObject->Load();
		}
	}
}

void Editor::Uninit()
{
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_editorObjectList[i])
		{
			gameObject->Uninit();
		}
		m_editorObjectList[i].clear();
	}
}

void Editor::Update()
{
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_editorObjectList[i])
		{
			// �A�N�e�B�u�t���O��ON�Ȃ�X�V����������
			if (gameObject->GetActive())
				gameObject->Update();
		}
	}
}

void Editor::Draw()
{
	// �`�������O�Ƀ��[�J���s�񂩂烏�[���h�s����쐬
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_editorObjectList[i])
		{
			auto transform = gameObject->GetTransform();
			// �e�̂Ȃ��I�u�W�F�N�g�̃��[���h�s��쐬�֐����ĂсA�q�I�u�W�F�N�g�֘A��������
			if (transform->GetParent() == nullptr)
				transform->MakeWorldMatrix(nullptr);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_editorObjectList[i])
		{
			// �A�N�e�B�u�t���O��ON�Ȃ�`�悷��
			if (gameObject->GetActive())
				gameObject->Draw();
		}
	}
}

void Editor::CheckDestroyedObject()
{
	for (int i = 0; i < 3; i++)
	{
		// �I�u�W�F�N�g�̔j���t���O���`�F�b�N
		m_editorObjectList[i].remove_if([](const auto& object)
		{return object->Destroy(); });
	}
	// �j������Ȃ��I�u�W�F�N�g�̃R���|�[�l���g�̔j���t���O���`�F�b�N
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_editorObjectList[i]) {
			gameObject->CheckDestroyedComponent();
		}
	}
}
