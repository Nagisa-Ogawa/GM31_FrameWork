#include "ObjectTreeNode.h"
#include "gameObject.h"


/// <summary>
/// 子ノードを作成し、挿入する関数
/// </summary>
/// <param name="m_object">子ノードのオブジェクト</param>
/// <returns>作成した子ノード</returns>
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
/// 子ノードを直接挿入する関数
/// </summary>
/// <param name="child">子ノード</param>
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
/// IDから子ノードを削除する関数
/// </summary>
/// <param name="childID">子ノードのID</param>
void ObjectTreeNode::DeleteChild(int childID)
{
    m_childList.remove_if([&childID](ObjectTreeNode* child) {return child->GetID() == childID; });
    return;
}


/// <summary>
/// IDからノードを検索する関数
/// </summary>
/// <param name="id">検索するID</param>
/// <param name="objectNode">見つかったノード</param>
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
