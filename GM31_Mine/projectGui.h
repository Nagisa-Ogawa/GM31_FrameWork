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
	int m_registerID = -1;
public:
	void Init() override;	// 初期化関数
	void Update()override;	// 毎フレーム処理する関数

	void ShowDirNode(FileTreeNode* fileNode, int* selectNode);		// ディレクトリのノードを表示する関数
	void ShowSelectChildNode(FileTreeNode* fileNode);				// 選択されたノードの子ノードを表示する関数
	std::string CutFileName(std::string fileName, int maxWidth);	// ファイル名が長すぎる場合に省略する関数
	std::string GetFileExtension(std::string fileName);				// ファイルの拡張子を取得する関数
	std::string GetNoFileExtension(std::string fileName);			// ファイルの拡張子を削った文字列を取得する関数
	void FindFile(FileTreeNode* fileNode, int* registerID);			// ファイルを検索し、ファイル木構造を作成する関数
	void UpdateFile(FileTreeNode* fileNode);						// ファイル木構造を更新する
	void PushDirButton(FileTreeNode* fileNode);						// ディレクトリボタンを押した際に呼ばれる関数
	void PushFileButton(FileTreeNode* fileNode);					// ファイルボタンを押した際に呼ばれる関数
	void PushReloadButton();										// リロードボタンを押した際に呼ばれる関数
	void ShowCreateScriptPopup();									// スクリプトファイルを作成するポップアップを表示する関数
	void ShowCreateScenePopup();									// シーンファイルを作成するポップアップを表示する関数
	void SetFileNode(FileTreeNode* fileNode);						// 選択されたているノードを変更する関数
};


