#pragma once
#include <string>
#include <list>

enum FILENODE_TYPE
{
	NODE_DIRECTORY,
	NODE_FILE,
};

class FileTreeNode
{
private:
	int m_id = -1;		// ID
	FileTreeNode* m_parent;	// �e
	std::list<FileTreeNode*> m_childList;	// �q�̃��X�g
	std::string m_fileName;				// �t�@�C���̖��O
	std::string m_fileFullPath;				// �t�@�C���̃t���p�X
	FILENODE_TYPE m_fileType;					// �t�@�C���̎��

public:
	int GetID() { return m_id; }
	FileTreeNode* GetParent() { return m_parent; }
	std::list<FileTreeNode*> GetChild() { return m_childList; }
	std::string GetFileName() { return m_fileName; }
	std::string GetFileFullPath() { return m_fileFullPath; }
	FILENODE_TYPE GetFileType() { return m_fileType; }

	void SetID(int id) { m_id = id; }
	void SetParent(FileTreeNode* parent) { m_parent = parent; }
	void SetFileName(std::string fileName) { m_fileName = fileName; }
	void SetFileFullPath(std::string fullpath) { m_fileFullPath = fullpath; }
	void SetFileType(FILENODE_TYPE fileType) { m_fileType = fileType; }

	FileTreeNode* Insert(int id, FILENODE_TYPE fileType, std::string fileName);
	bool Delete();

	bool IsExistSameFileName(std::string fileName);	// �����̃t�@�C�����Ɠ����t�@�C�����̎q�������邩�`�F�b�N����֐�
	bool IsExistDirChild();							// �f�B���N�g���̎q�������݂��邩���`�F�b�N����֐�
	std::list<FileTreeNode*> GetDirChildList();		// �f�B���N�g���̎q���̃��X�g��Ԃ��֐�
};