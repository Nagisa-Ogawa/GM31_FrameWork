#pragma once
#include <string>
#include "MyImGui.h"

class ObjectTreeNode;


//-----------------------------------------------------
// �q�G�����L�[�E�B���h�E�pGUI�N���X
//------------------------------------------------------
class HierarchyGui : public MyImGui
{
private:
	ObjectTreeNode* m_sceneObjectTree = nullptr;	// �V�[���̃I�u�W�F�N�g��\���؍\��
	// ObjectTreeNode* m_selectNode = nullptr;			// �I�������I�u�W�F�N�g�m�[�h
	int m_selectionMask = 0;
	int m_selectionID = -1;							// �I�����ꂽ�m�[�h��ID

public:
	void Init() override;	// �������֐�
	void Update()override;	// ���t���[����������֐�

	void ShowObjectTree();	// �V�[���̃I�u�W�F�N�g�c���[��\������֐�
	void ShowObjectNode(ObjectTreeNode* objectNode, int* selectionID);	// �I�u�W�F�N�g�m�[�h��\������֐�

	void AddObjectNode(GameObject* object);		// �I�u�W�F�N�g�c���[�ɐV�����I�u�W�F�N�g��ǉ�����֐�
	void ChangeParentNode(int nodeID, int parentID, int nextParentID);	// �I�u�W�F�N�g�̐e�m�[�h��ύX����֐�
	void DeleteObjectNode(int objectID);					// �I�u�W�F�N�g�m�[�h���폜����֐�

	void InitObjectTree();		// �V�[���̃I�u�W�F�N�g�c���[������������֐�
	void CreateObjectNode(ObjectTreeNode* objectNode);		// �I�u�W�F�N�g�̃c���[�m�[�h���쐬����֐�
	void SetSelect(GameObject* object);
};




