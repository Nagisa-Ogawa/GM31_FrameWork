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
	FileTreeNode* m_parent;	// 親
	std::list<FileTreeNode*> m_childList;	// 子のリスト
	std::string m_fileName;				// ファイルの名前
	std::string m_fileFullPath;				// ファイルのフルパス
	FILENODE_TYPE m_fileType;					// ファイルの種類

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

	bool IsExistSameFileName(std::string fileName);	// 引数のファイル名と同じファイル名の子供がいるかチェックする関数
	bool IsExistDirChild();							// ディレクトリの子供が存在するかをチェックする関数
	std::list<FileTreeNode*> GetDirChildList();		// ディレクトリの子供のリストを返す関数
};