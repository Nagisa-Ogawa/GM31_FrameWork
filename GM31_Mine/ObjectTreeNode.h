#pragma once
#include <string>
#include <list>

class GameObject;


//-----------------------------------------------------
// �I�u�W�F�N�g�p�؍\���N���X
//------------------------------------------------------
class ObjectTreeNode
{
private:
	int m_id;
	std::string m_name;
	ObjectTreeNode* m_parent;	// �e
	std::list<ObjectTreeNode*> m_childList;	// �q�̃��X�g
	GameObject* m_object;
public:
	// Get�n�֐�
	int GetID() { return m_id; }
	std::string GetName() { return m_name; }
	ObjectTreeNode* GetParent() { return m_parent; }
	std::list<ObjectTreeNode*> GetChild() { return m_childList; }
	GameObject* GetTreeObject() { return m_object; }
	// Set�n�֐�
	void SetID(int id) { m_id = id; }
	void SetName(std::string name) { m_name = name; }
	void SetParent(ObjectTreeNode* parent) { m_parent = parent; }
	void SetTreeObject(GameObject* object) { m_object = object; }

	ObjectTreeNode* Insert(GameObject* m_object);	// �m�[�h��}��
	void InsertChild(ObjectTreeNode* child);		// �q�m�[�h�𒼐ڑ}��
	bool Delete(bool isDeleteThis);			// �m�[�h�폜
	void DeleteChild(int childID);			// ID����q�m�[�h���폜����֐�
	ObjectTreeNode* FindNode(int id);	// ID����m�[�h������

};