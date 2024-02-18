#pragma once
#include <string>
#include <list>

class GameObject;


//-----------------------------------------------------
// オブジェクト用木構造クラス
//------------------------------------------------------
class ObjectTreeNode
{
private:
	int m_id;
	std::string m_name;
	ObjectTreeNode* m_parent;	// 親
	std::list<ObjectTreeNode*> m_childList;	// 子のリスト
	GameObject* m_object;
public:
	// Get系関数
	int GetID() { return m_id; }
	std::string GetName() { return m_name; }
	ObjectTreeNode* GetParent() { return m_parent; }
	std::list<ObjectTreeNode*> GetChild() { return m_childList; }
	GameObject* GetTreeObject() { return m_object; }
	// Set系関数
	void SetID(int id) { m_id = id; }
	void SetName(std::string name) { m_name = name; }
	void SetParent(ObjectTreeNode* parent) { m_parent = parent; }
	void SetTreeObject(GameObject* object) { m_object = object; }

	ObjectTreeNode* Insert(GameObject* m_object);	// ノードを挿入
	void InsertChild(ObjectTreeNode* child);		// 子ノードを直接挿入
	bool Delete(bool isDeleteThis);			// ノード削除
	void DeleteChild(int childID);			// IDから子ノードを削除する関数
	ObjectTreeNode* FindNode(int id);	// IDからノードを検索

};