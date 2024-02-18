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
// インスペクターウィンドウ用GUIクラス
//------------------------------------------------------
class InspectorGui : public MyImGui
{
private:
	SHOW_MODE m_showMode;
	GameObject* m_selectedObject = nullptr;
	FileTreeNode* m_fileNode = nullptr;
	bool m_isAddComp = false;
public:
	void Init() override;	// 初期化関数
	void Update()override;	// 毎フレーム処理する関数

	GameObject* GetSelectedObject() { return m_selectedObject; }
	void SetSelectedObject(GameObject* object) { m_selectedObject = object; m_showMode = SHOW_MODE::SHOW_GAMEOBJECT; }
	void SetFileNode(FileTreeNode* fileNode) { m_fileNode = fileNode; m_showMode = SHOW_MODE::SHOW_FILE; }
};


