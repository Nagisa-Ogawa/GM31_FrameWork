#pragma once
#include "MyImGui.h"

enum SHOW_MODE
{
	SHOW_GAMEOBJECT,
	SHOW_FILE,
};

class GameObject;
class FileTreeNode;


//-----------------------------------------------------
// �C���X�y�N�^�[�E�B���h�E�pGUI�N���X
//------------------------------------------------------
class InspectorGui : public MyImGui
{
private:
	SHOW_MODE m_showMode;
	GameObject* m_selectedObject = nullptr;
	FileTreeNode* m_fileNode = nullptr;
	bool m_isAddComp = false;
public:
	void Init() override;	// �������֐�
	void Update()override;	// ���t���[����������֐�

	GameObject* GetSelectedObject() { return m_selectedObject; }
	void SetSelectedObject(GameObject* object) { m_selectedObject = object; m_showMode = SHOW_MODE::SHOW_GAMEOBJECT; }
	void SetFileNode(FileTreeNode* fileNode) { m_fileNode = fileNode; m_showMode = SHOW_MODE::SHOW_FILE; }
};


