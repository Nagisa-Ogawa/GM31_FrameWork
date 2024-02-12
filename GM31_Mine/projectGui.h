#pragma once
#include <map>
#include "MyImGui.h"

class FileTreeNode;

class ProjectGui : public MyImGui
{
private:
	FileTreeNode* m_fileTree = nullptr;
	FileTreeNode* m_selectNode = nullptr;
	std::map<std::string, ID3D11ShaderResourceView*> m_fileNodeTextureMap;
	ID3D11ShaderResourceView* m_plusButtonTexture = nullptr;
	int m_selectionMask = -1;
	int m_selectionID = -1;
public:
	void Init() override;	// �������֐�
	void Update()override;	// ���t���[����������֐�

	void ShowDirNode(FileTreeNode* fileNode, int* selectNode);		// �f�B���N�g���̃m�[�h��\������֐�
	void ShowSelectChildNode(FileTreeNode* fileNode);				// �I�����ꂽ�m�[�h�̎q�m�[�h��\������֐�
	std::string CutFileName(std::string fileName, int maxWidth);	// �t�@�C��������������ꍇ�ɏȗ�����֐�
	std::string GetFileExtension(std::string fileName);				// �t�@�C���̊g���q���擾����֐�
	void FindFile(FileTreeNode* fileNode, int* registerID);			// �t�@�C������������֐�
	void PushDirButton(FileTreeNode* fileNode);						// �f�B���N�g���{�^�����N���b�N�����ۂ̏���
	void PushFileButton(FileTreeNode* fileNode);					// �t�@�C���{�^�����N���b�N�����ۂ̏���
	void ShowCreateScriptPopup();										// �X�N���v�g�t�@�C�����쐬����|�b�v�A�b�v��\������֐�
	void ShowCreateScenePopup();										// �V�[���t�@�C�����쐬����|�b�v�A�b�v��\������֐�
	void SetFileNode(FileTreeNode* fileNode);					// �I�����ꂽ�Ă���m�[�h��ύX����֐�
};


