#include "FileTreeNode.h"

FileTreeNode* FileTreeNode::Insert(int id, FILENODE_TYPE fileType, std::string fileName)
{
    FileTreeNode* child = new FileTreeNode();
    child->m_id = id;
    child->m_fileType = fileType;
    child->m_fileName = fileName;
    child->m_parent = this;
    std::string fullPath;
    child->m_fileFullPath = m_fileFullPath + "\\" + fileName;
    m_childList.push_back(child);
    return child;
}

bool FileTreeNode::Delete()
{
    for (auto child : m_childList) {
        child->Delete();
    }
    delete this;
    return true;
}

/// <summary>
/// �����̃t�@�C�����Ɠ����t�@�C�����̎q�������邩�`�F�b�N����֐�
/// </summary>
/// <param name="fileName"></param>
/// <returns></returns>
bool FileTreeNode::IsExistSameFileName(std::string fileName)
{
    for (auto child : m_childList) {
        if (child->m_fileName == fileName) {
            return true;
        }
    }
    return false;
}

/// <summary>
/// �q�m�[�h�Ƀf�B���N�g���̃m�[�h�����邩���`�F�b�N����֐�
/// </summary>
/// <returns>�q�m�[�h�Ƀf�B���N�g���̃m�[�h�����݂������ǂ���</returns>
bool FileTreeNode::IsExistDirChild()
{
    for (auto child : m_childList) {
        if (child->m_fileType == FILENODE_TYPE::NODE_DIRECTORY) {
            return true;
        }
    }
    return false;
}

/// <summary>
/// �q�m�[�h�̃f�B���N�g���̃m�[�h�����X�g�ɂ��A�Ԃ��֐�
/// </summary>
/// <returns></returns>
std::list<FileTreeNode*> FileTreeNode::GetDirChildList()
{
    std::list<FileTreeNode*> dirList;
    for (auto child : m_childList) {
        if (child->m_fileType == FILENODE_TYPE::NODE_DIRECTORY) {
            dirList.push_back(child);
        }
    }
    return dirList;
}
