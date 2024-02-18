#pragma once
#include <string>
#include "MyImGui.h"

class ObjectTreeNode;


//-----------------------------------------------------
// ヒエラルキーウィンドウ用GUIクラス
//------------------------------------------------------
class HierarchyGui : public MyImGui
{
private:
	ObjectTreeNode* m_sceneObjectTree = nullptr;	// シーンのオブジェクトを表す木構造
	// ObjectTreeNode* m_selectNode = nullptr;			// 選択したオブジェクトノード
	int m_selectionMask = 0;
	int m_selectionID = -1;							// 選択されたノードのID

public:
	void Init() override;	// 初期化関数
	void Update()override;	// 毎フレーム処理する関数

	void ShowObjectTree();	// シーンのオブジェクトツリーを表示する関数
	void ShowObjectNode(ObjectTreeNode* objectNode, int* selectionID);	// オブジェクトノードを表示する関数

	void AddObjectNode(GameObject* object);		// オブジェクトツリーに新しいオブジェクトを追加する関数
	void ChangeParentNode(int nodeID, int parentID, int nextParentID);	// オブジェクトの親ノードを変更する関数
	void DeleteObjectNode(int objectID);					// オブジェクトノードを削除する関数

	void InitObjectTree();		// シーンのオブジェクトツリーを初期化する関数
	void CreateObjectNode(ObjectTreeNode* objectNode);		// オブジェクトのツリーノードを作成する関数
	void SetSelect(GameObject* object);
};




