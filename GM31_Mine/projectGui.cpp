
#include "main.h"
#include "renderer.h"
#include "MyImGuiManager.h"
#include "ImGui/imgui.h"
#include "FileTreeNode.h"
#include "inspectorGui.h"
#include "SceneGui.h"
#include "manager.h"
#include "scene.h"
#include "projectGui.h"
#include "LuaManager.h"

#define ICON_NUM (7)
#define BUTTON_SIZE_FILE (70)
#define CREATE_SCENE_POPUP_NAME ("Create Scene")
#define CREATE_SCRIPT_POPUP_NAME ("Create Script")


void ProjectGui::Init()
{
    // ファイルのアイコンとなる画像を読み込み
    ID3D11ShaderResourceView* m_iconTextures[ICON_NUM];
    std::string iconPaths[ICON_NUM] = { "folder","obj","jpg","png","json","lua","hlsl" };
    for (int i = 0; i < ICON_NUM; i++) {
        std::string path = "Assets\\Textures\\" + iconPaths[i] + "Icon.png";
        D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
            path.c_str(),
            NULL,
            NULL,
            &m_iconTextures[i],
            NULL);
        assert(m_iconTextures[i]);
        std::string key = "." + iconPaths[i];
        m_fileNodeTextureMap[key] = m_iconTextures[i];
    }
    // プラスボタンの画像を読み込み
    D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
        "Assets\\Textures\\plus.png",
        NULL,
        NULL,
        &m_plusButtonTexture,
        NULL);
    assert(m_plusButtonTexture);

    m_selectionMask = 0;
    // 一番親となるノードを作成
    m_fileTree = new FileTreeNode();
    m_fileTree->SetID(0);
    m_fileTree->SetFileName("Assets");
    m_fileTree->SetFileFullPath("Assets");
    m_fileTree->SetFileType(FILENODE_TYPE::NODE_DIRECTORY);
    m_registerID = 1;
    // ファイルを検索
    FindFile(m_fileTree,&m_registerID);
}

void ProjectGui::Update()
{
	ImGui::Begin("Project");

	MyImGuiManager::GetInstance()->SetFocusWindow(ImGui::GetCurrentWindow());
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
	ImGui::BeginChild("Dir", ImVec2(ImGui::GetContentRegionAvail().x * 0.1f, 0),true,ImGuiWindowFlags_ChildWindow);
    // 木構造をリロードする関数
    if (ImGui::Button("Reload", ImVec2(60, 20))) {
        PushReloadButton();
    }
    // ファイルの木構造を表示
    ShowDirNode(m_fileTree,&m_selectionID);
    // 選択されたノードがあるかチェック
    if (m_selectionID != -1)
    {
        m_selectionMask = (1 << m_selectionID);
    }
    ImGui::EndChild();

	ImGui::SameLine();
    ImGui::BeginChild("File", ImVec2(0, 0), true, 0);
	// 選択されたノードがある場合そのノードの子要素を表示
    if (m_selectNode) {
        ShowSelectChildNode(m_selectNode);
    }
	ImGui::EndChild();

    ImGui::PopStyleVar();
	ImGui::End();
}


/// <summary>
/// 木構造のファイルノードを選択する関数
/// </summary>
/// <param name="fileNode">表示するファイルノード</param>
/// <param name="selectNode">選択されたノードのID</param>
void ProjectGui::ShowDirNode(FileTreeNode* fileNode,int* selectNodeID)
{
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    // このノードが選択されているなら選択フラグをONにする
    const bool isSelected = (m_selectionMask & (1 << fileNode->GetID())) != 0;
    if (isSelected)
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    // このノードにフォルダノードがあるなら子供がいる時用のノードにする
    if (fileNode->IsExistDirChild()) {
        // 子供がいる時のツリーノード
        nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
        bool isNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)fileNode->GetID(), nodeFlags, fileNode->GetFileName().c_str(), fileNode->GetID());
        // ノードがクリックされたなら選択状態にする
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            *selectNodeID = fileNode->GetID();
            m_selectNode = fileNode;
        }
        // ノードが展開されているなら
        if (isNodeOpen)
        {
            // このノードの子ノードかつフォルダであるノードを表示
            auto dirList = fileNode->GetDirChildList();
            for (auto child : dirList) {
                ShowDirNode(child, selectNodeID);
            }
            ImGui::TreePop();
        }
    }
    else {
        // 子供がいないなら三角形がついていないノードにする
        nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)fileNode->GetID(), nodeFlags, fileNode->GetFileName().c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            *selectNodeID = fileNode->GetID();
            m_selectNode = fileNode;
        }
    }

}

/// <summary>
/// 選択されたノードの子ノードを表示する関数
/// </summary>
/// <param name="fileNode"></param>
void ProjectGui::ShowSelectChildNode(FileTreeNode* fileNode)
{
    ImVec2 btnSize(BUTTON_SIZE_FILE, BUTTON_SIZE_FILE);
    ImGuiStyle& style = ImGui::GetStyle();
    // ボタンのスタイルを変更
    style.Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    float windowVisibleSize = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    int n = 0;
    int lastID = -1;
    int btnCount = fileNode->GetChild().size();
    if (m_selectNode->GetFileName() == "Scenes" || m_selectNode->GetFileName() == "Scripts") {
        btnCount++;
    }
    // 現在のディレクトリの子ファイルを全て表示
    for (auto child : fileNode->GetChild())
    {
        ImGui::PushID(child->GetID());
        ImGui::BeginGroup();
        // ディレクトリボタンが押されたとき
        if (child->GetFileType() == FILENODE_TYPE::NODE_DIRECTORY) {
            ImGui::ImageButton((void*)m_fileNodeTextureMap[".folder"], btnSize);
            // ボタンをダブルクリックしたとき
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                PushDirButton(child);
            }
        }
        // ファイルボタンが押されたとき
        else {
            // ファイルの拡張子によって表示するテクスチャボタンを変える
            std::string extName = GetFileExtension(child->GetFileName());
            if (ImGui::ImageButton((void*)m_fileNodeTextureMap[extName], btnSize)) {
                PushFileButton(child);
            }
            // ファイルボタンをダブルクリックした時
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                // シーンファイルなら
                if (m_selectNode->GetFileName() == "Scenes" && extName == ".json") {
                    PushSceneFileButton(child);
                }
            }
        }
        // ファイル名が長すぎる場合省略
        std::string fileName = CutFileName(child->GetFileName(),btnSize.x);
        // ファイル名を表示
        ImGui::Text(fileName.c_str());
        ImGui::EndGroup();
        float lastButtonPos = ImGui::GetItemRectMax().x;
        float nextButtonPos = lastButtonPos + style.ItemSpacing.x + btnSize.x;
        // 次のボタンの位置がウィンドウからはみ出さないなら同じ行に配置
        if (n + 1 < btnCount && nextButtonPos < windowVisibleSize)
            ImGui::SameLine(0.0f,20.0f);
        ImGui::PopID();
        n++;
    }
    // シーンフォルダを開いている場合シーン追加ボタンを表示
    if (m_selectNode->GetFileName() == "Scenes") {
        ImGui::ImageButton((void*)m_plusButtonTexture, btnSize);
        // ボタンをダブルクリックした時
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            ImGui::OpenPopup(CREATE_SCENE_POPUP_NAME);
        }
    }
    // スクリプトフォルダを開いている場合スクリプト追加ボタンを表示
    else if (m_selectNode->GetFileName() == "Scripts") {
        ImGui::ImageButton((void*)m_plusButtonTexture, btnSize);
        // ボタンをダブルクリックした時
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            ImGui::OpenPopup(CREATE_SCRIPT_POPUP_NAME);
        }
    }
    // スタイルをリセット
    style = ImGuiStyle();
    ShowCreateScenePopup();
    ShowCreateScriptPopup();

}

/// <summary>
/// ファイル名が長すぎる場合省略する関数
/// </summary>
/// <param name="fileName">省略する関数</param>
/// <param name="maxWidth">省略する長さの限界値</param>
/// <returns>省略したファイル名</returns>
std::string ProjectGui::CutFileName(std::string fileName,int maxWidth)
{
    std::string cutName = fileName;
    std::string cutSymbol = "..";
    float cutTextWidth = ImGui::CalcTextSize(cutSymbol.c_str()).x;
    // ファイル名が既定値の長さになるまで文字を省略
    while (ImGui::CalcTextSize(cutName.c_str()).x > (maxWidth - cutTextWidth)) {
        cutName.pop_back();
    };
    // ファイル名を省略していたなら省略記号をつける
    if (fileName != cutName)
        cutName += cutSymbol;
    return cutName;
}

std::string ProjectGui::GetFileExtension(std::string fileName)
{
    std::string extName = "";
    int extNum = fileName.find_last_of(".");
    if (extNum != -1)
        extName = fileName.substr(extNum, fileName.size() - extNum);
    return extName;
}

std::string ProjectGui::GetNoFileExtension(std::string fileName)
{
    std::string noExtName = "";
    int extNum = fileName.find_last_of(".");
    if (extNum != -1)
        noExtName = fileName.substr(0, extNum);
    return noExtName;
}

/// <summary>
/// 指定されたディレクトリにあるフォルダやファイルを木構造に格納する関数
/// </summary>
/// <param name="fileNode">格納するノード</param>
/// <param name="registerID">格納するノードのID</param>
void ProjectGui::FindFile(FileTreeNode* fileNode, int* registerID)
{
    HANDLE fHandle;
    WIN32_FIND_DATA win32fd;

    std::string searchName = fileNode->GetFileFullPath()+"\\*";

    fHandle = FindFirstFile(searchName.c_str(), &win32fd);

    // ファイルがないなら終了
    if (fHandle == INVALID_HANDLE_VALUE) {
        // "'GetLastError() == 3' is 'file not found'"
        return;
    }

    do {
        if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            //ディレクトリである
            //そのディレクトリそのものを表す場合は処理しない
            if (win32fd.cFileName[0] == '.') continue;
            auto childNode = fileNode->Insert(*registerID, FILENODE_TYPE::NODE_DIRECTORY, win32fd.cFileName);
            (*registerID)++;
            //再帰的にファイルを検索
            FindFile(childNode,registerID);
        }
        else {
            //ファイルである
            // 拡張子が.obj .png .jpg .jsonなら格納
            std::string fileName = win32fd.cFileName;
            std::string extName = GetFileExtension(fileName);
            if (extName != ".obj" && extName != ".png" && extName != ".jpg" && extName != ".json" && extName != ".lua" && extName != ".hlsl") continue;
            // ノードに挿入
            fileNode->Insert((*registerID), FILENODE_TYPE::NODE_FILE, win32fd.cFileName);
            (*registerID)++;
        }
    } while (FindNextFile(fHandle, &win32fd));

    FindClose(fHandle);
}


/// <summary>
/// ファイル木構造を更新する関数
/// </summary>
/// <param name="fileNode"></param>
void ProjectGui::UpdateFile(FileTreeNode* fileNode)
{

}


/// <summary>
/// ディレクトリボタンを押した時の処理
/// </summary>
/// <param name="fileNode"></param>
void ProjectGui::PushDirButton(FileTreeNode* fileNode)
{
    SetFileNode(fileNode);
}


/// <summary>
/// ファイルボタンを押した時の処理
/// </summary>
/// <param name="fileNode"></param>
void ProjectGui::PushFileButton(FileTreeNode* fileNode)
{
    auto inspector = MyImGuiManager::GetInstance()->GetImGui<InspectorGui>();
    inspector->SetFileNode(fileNode);
}


/// <summary>
/// シーンファイルボタンを押した時の処理
/// </summary>
/// <param name="fileNode">選択したノードの情報</param>
void ProjectGui::PushSceneFileButton(FileTreeNode* fileNode)
{
    std::string sceneName = GetNoFileExtension(fileNode->GetFileName());
    // シーンを読み込み
    Manager::GetInstance()->ChangeScene(sceneName);
}


/// <summary>
/// ファイル再読み込みボタンを押した時の処理
/// </summary>
void ProjectGui::PushReloadButton()
{
    auto inspector = MyImGuiManager::GetInstance()->GetImGui<InspectorGui>();
    inspector->SetFileNode(nullptr);
    m_selectNode = nullptr;
    m_selectionID = -1;
    m_selectionMask = 0;
    m_fileTree->Delete(false);
    m_registerID = 1;
    FindFile(m_fileTree, &m_registerID);
}


/// <summary>
/// シーンを作成するポップアップを表示する関数
/// </summary>
void ProjectGui::ShowCreateScenePopup()
{
    // シーンを作成するポップアップを表示
    if (ImGui::BeginPopupModal(CREATE_SCENE_POPUP_NAME, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
        ImGui::Separator();
        static char str[128] = "";
        ImGui::InputText(".json", str, IM_ARRAYSIZE(str));
        ImVec2 btnSize = ImVec2(60, 30);
        if (ImGui::Button("OK", btnSize)) {
            std::string fileName = str;
            // 拡張子があるなら外す
            if (GetFileExtension(fileName) == ".json") {
                fileName = GetNoFileExtension(fileName);
            }
            // ファイル名が入力されているかチェック
            if (fileName == "") {
                MyImGuiManager::GetInstance()->DebugLog("Faild!! This scene name is enpty!!");
            }
            // 同じシーン名があるかチェック
            else if (m_selectNode->IsExistSameFileName(fileName+".json")) {
                MyImGuiManager::GetInstance()->DebugLog("Faild!! This scene name is already in use!!");
            }
            else {
                // シーンを作成する
                if (Manager::GetInstance()->CreateScene(fileName)) {
                    // スクリプトを作成したならファイル木構造に追加
                    m_selectNode->Insert(m_registerID, FILENODE_TYPE::NODE_FILE, fileName + ".json");
                    m_registerID++;
                    // 成功したことを通知
                    MyImGuiManager::GetInstance()->DebugLog("Success!! Create Scene!!");
                }
            }
            strcpy(str, "");
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine(250);
        if (ImGui::Button("Cancel", btnSize)) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleVar();
        ImGui::EndPopup();
    }
}


/// <summary>
/// スクリプトを作成するポップアップを表示する関数
/// </summary>
void ProjectGui::ShowCreateScriptPopup()
{
    // スクリプトを作成するポップアップを表示
    if (ImGui::BeginPopupModal(CREATE_SCRIPT_POPUP_NAME,NULL,ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
        ImGui::Separator();
        static char str[128] = "";
        ImGui::InputText(".lua", str, IM_ARRAYSIZE(str));
        ImVec2 btnSize = ImVec2(60, 30);
        if (ImGui::Button("OK", btnSize)) {
            std::string fileName = str;
            // 拡張子がないなら付け足す
            if (GetFileExtension(fileName) != ".lua") {
                fileName += ".lua";
            }
            // ファイル名が入力されているかチェック
            if (fileName == ".lua") {
                MyImGuiManager::GetInstance()->DebugLog("Faild!! This file name is enpty!!");
            }
            // 同じファイル名があるかチェック
            else if (m_selectNode->IsExistSameFileName(fileName)) {
                MyImGuiManager::GetInstance()->DebugLog("Faild!! This file name is already in use!!");
            }
            else {
                // スクリプトを作成する
                if (LuaManager::GetInstance()->CreateScriptFile(GetNoFileExtension(fileName))) {
                    // スクリプトを作成したならファイル木構造に追加
                    m_selectNode->Insert(m_registerID, FILENODE_TYPE::NODE_FILE, fileName);
                    m_registerID++;
                    // 成功したことを通知
                    MyImGuiManager::GetInstance()->DebugLog("Success!! Create LuaScript!!");
                }
            }
            strcpy(str, "");
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine(250);
        if (ImGui::Button("Cancel", btnSize)) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleVar();
        ImGui::EndPopup();
    }
}



void ProjectGui::SetFileNode(FileTreeNode* fileNode)
{
    m_selectNode = fileNode;
    m_selectionID = fileNode->GetID();
}

