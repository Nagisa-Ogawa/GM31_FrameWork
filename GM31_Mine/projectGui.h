#pragma once
#include <map>
#include "MyImGui.h"

class FileTreeNode;


//-----------------------------------------------------
// �v���W�F�N�g�E�B���h�E�pGUI�N���X
//------------------------------------------------------
// Assets�t�@�C���ɕύX���������ꍇ
// Reload�{�^���������܂ōX�V������܂���
//------------------------------------------------------
class ProjectGui : public MyImGui
{
private:
	FileTreeNode* m_fileTree = nullptr;		// �t�@�C���̖؍\����\���c���[�m�[�h
	FileTreeNode* m_selectNode = nullptr;	// �I�����ꂽ�t�@�C���̃c���[�m�[�h
	std::map<std::string, ID3D11ShaderResourceView*> m_fileNodeTextureMap;	// �t�@�C���̊g���q������Ƃ��̉摜���\�[�X�̘A�z�z��
	ID3D11ShaderResourceView* m_plusButtonTexture = nullptr;	// �{�{�^���p�摜���\�[�X
	int m_selectionMask = -1;
	int m_selectionID = -1;		// �I�����ꂽ�m�[�h��ID
	int m_registerID = -1;		// �؍\���ɓo�^����ID
public:
	void Init() override;	// �������֐�
	void Update()override;	// ���t���[����������֐�

	void ShowDirNode(FileTreeNode* fileNode, int* selectNode);		// �f�B���N�g���̃m�[�h��\������֐�
	void ShowSelectChildNode(FileTreeNode* fileNode);				// �I�����ꂽ�m�[�h�̎q�m�[�h��\������֐�

	std::string CutFileName(std::string fileName, int maxWidth);	// �t�@�C��������������ꍇ�ɏȗ�����֐�
	std::string GetFileExtension(std::string fileName);				// �t�@�C���̊g���q���擾����֐�
	std::string GetNoFileExtension(std::string fileName);			// �t�@�C���̊g���q���������������擾����֐�
	void FindFile(FileTreeNode* fileNode, int* registerID);			// �t�@�C�����������A�t�@�C���؍\�����쐬����֐�
	void UpdateFile(FileTreeNode* fileNode);						// �t�@�C���؍\�����X�V����

	void PushDirButton(FileTreeNode* fileNode);						// �f�B���N�g���{�^�����������ۂɌĂ΂��֐�
	void PushFileButton(FileTreeNode* fileNode);					// �t�@�C���{�^�����������ۂɌĂ΂��֐�
	void PushSceneFileButton(FileTreeNode* fileNode);				// �V�[���t�@�C���{�^�����������ۂɌĂ΂��֐�
	void PushReloadButton();										// �����[�h�{�^�����������ۂɌĂ΂��֐�
	void ShowCreateScriptPopup();									// �X�N���v�g�t�@�C�����쐬����|�b�v�A�b�v��\������֐�
	void ShowCreateScenePopup();									// �V�[���t�@�C�����쐬����|�b�v�A�b�v��\������֐�
	void SetFileNode(FileTreeNode* fileNode);						// �I�����ꂽ�Ă���m�[�h��ύX����֐�
};


