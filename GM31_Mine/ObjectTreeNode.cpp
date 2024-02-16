#include "ObjectTreeNode.h"
#include "gameObject.h"


/// <summary>
/// �q�m�[�h���쐬���A�}������֐�
/// </summary>
/// <param name="m_object">�q�m�[�h�̃I�u�W�F�N�g</param>
/// <returns>�쐬�����q�m�[�h</returns>
ObjectTreeNode* ObjectTreeNode::Insert(GameObject* m_object)
{
    ObjectTreeNode* child = new ObjectTreeNode();
    child->m_id = m_object->GetID();
    child->m_name = m_object->GetName();
    child->m_parent = this;
    child->m_object = m_object;
    m_childList.push_back(child);
    return child;
}


/// <summary>
/// �q�m�[�h�𒼐ڑ}������֐�
/// </summary>
/// <param name="child">�q�m�[�h</param>
void ObjectTreeNode::InsertChild(ObjectTreeNode* child)
{
    m_childList.push_back(child);
}

bool ObjectTreeNode::Delete(bool isDeleteThis)
{
    for (auto child : m_childList) {
        child->Delete(true);
    }
    m_childList.clear();
    if (isDeleteThis) {
        delete this;
    }
    return true;
}


/// <summary>
/// ID����q�m�[�h���폜����֐�
/// </summary>
/// <param name="childID">�q�m�[�h��ID</param>
void ObjectTreeNode::DeleteChild(int childID)
{
    m_childList.remove_if([&childID](ObjectTreeNode* child) {return child->GetID() == childID; });
    return;
}


/// <summary>
/// ID����m�[�h����������֐�
/// </summary>
/// <param name="id">��������ID</param>
/// <param name="objectNode">���������m�[�h</param>
/// <returns></returns>
ObjectTreeNode* ObjectTreeNode::FindNode(int id)
{
    if (m_id == id) {
        return this;
    }
    for (auto child : m_childList) {
        auto node = child->FindNode(id);
        if (node) {
            return node;
        }
    }
}
