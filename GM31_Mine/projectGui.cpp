
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
    // �t�@�C���̃A�C�R���ƂȂ�摜��ǂݍ���
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
    // �v���X�{�^���̉摜��ǂݍ���
    D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
        "Assets\\Textures\\plus.png",
        NULL,
        NULL,
        &m_plusButtonTexture,
        NULL);
    assert(m_plusButtonTexture);

    m_selectionMask = 0;
    // ��Ԑe�ƂȂ�m�[�h���쐬
    m_fileTree = new FileTreeNode();
    m_fileTree->SetID(0);
    m_fileTree->SetFileName("Assets");
    m_fileTree->SetFileFullPath("Assets");
    m_fileTree->SetFileType(FILENODE_TYPE::NODE_DIRECTORY);
    m_registerID = 1;
    // �t�@�C��������
    FindFile(m_fileTree,&m_registerID);
}

void ProjectGui::Update()
{
	ImGui::Begin("Project");

	MyImGuiManager::GetInstance()->SetFocusWindow(ImGui::GetCurrentWindow());
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
	ImGui::BeginChild("Dir", ImVec2(ImGui::GetContentRegionAvail().x * 0.1f, 0),true,ImGuiWindowFlags_ChildWindow);
    // �؍\���������[�h����֐�
    if (ImGui::Button("Reload", ImVec2(60, 20))) {
        PushReloadButton();
    }
    // �t�@�C���̖؍\����\��
    ShowDirNode(m_fileTree,&m_selectionID);
    // �I�����ꂽ�m�[�h�����邩�`�F�b�N
    if (m_selectionID != -1)
    {
        m_selectionMask = (1 << m_selectionID);
    }
    ImGui::EndChild();

	ImGui::SameLine();
    ImGui::BeginChild("File", ImVec2(0, 0), true, 0);
	// �I�����ꂽ�m�[�h������ꍇ���̃m�[�h�̎q�v�f��\��
    if (m_selectNode) {
        ShowSelectChildNode(m_selectNode);
    }
	ImGui::EndChild();

    ImGui::PopStyleVar();
	ImGui::End();
}


/// <summary>
/// �؍\���̃t�@�C���m�[�h��I������֐�
/// </summary>
/// <param name="fileNode">�\������t�@�C���m�[�h</param>
/// <param name="selectNode">�I�����ꂽ�m�[�h��ID</param>
void ProjectGui::ShowDirNode(FileTreeNode* fileNode,int* selectNodeID)
{
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    // ���̃m�[�h���I������Ă���Ȃ�I���t���O��ON�ɂ���
    const bool isSelected = (m_selectionMask & (1 << fileNode->GetID())) != 0;
    if (isSelected)
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    // ���̃m�[�h�Ƀt�H���_�m�[�h������Ȃ�q�������鎞�p�̃m�[�h�ɂ���
    if (fileNode->IsExistDirChild()) {
        // �q�������鎞�̃c���[�m�[�h
        nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
        bool isNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)fileNode->GetID(), nodeFlags, fileNode->GetFileName().c_str(), fileNode->GetID());
        // �m�[�h���N���b�N���ꂽ�Ȃ�I����Ԃɂ���
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            *selectNodeID = fileNode->GetID();
            m_selectNode = fileNode;
        }
        // �m�[�h���W�J����Ă���Ȃ�
        if (isNodeOpen)
        {
            // ���̃m�[�h�̎q�m�[�h���t�H���_�ł���m�[�h��\��
            auto dirList = fileNode->GetDirChildList();
            for (auto child : dirList) {
                ShowDirNode(child, selectNodeID);
            }
            ImGui::TreePop();
        }
    }
    else {
        // �q�������Ȃ��Ȃ�O�p�`�����Ă��Ȃ��m�[�h�ɂ���
        nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)fileNode->GetID(), nodeFlags, fileNode->GetFileName().c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            *selectNodeID = fileNode->GetID();
            m_selectNode = fileNode;
        }
    }

}

/// <summary>
/// �I�����ꂽ�m�[�h�̎q�m�[�h��\������֐�
/// </summary>
/// <param name="fileNode"></param>
void ProjectGui::ShowSelectChildNode(FileTreeNode* fileNode)
{
    ImVec2 btnSize(BUTTON_SIZE_FILE, BUTTON_SIZE_FILE);
    ImGuiStyle& style = ImGui::GetStyle();
    // �{�^���̃X�^�C����ύX
    style.Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    float windowVisibleSize = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    int n = 0;
    int lastID = -1;
    int btnCount = fileNode->GetChild().size();
    if (m_selectNode->GetFileName() == "Scenes" || m_selectNode->GetFileName() == "Scripts") {
        btnCount++;
    }
    // ���݂̃f�B���N�g���̎q�t�@�C����S�ĕ\��
    for (auto child : fileNode->GetChild())
    {
        ImGui::PushID(child->GetID());
        ImGui::BeginGroup();
        // �f�B���N�g���{�^���������ꂽ�Ƃ�
        if (child->GetFileType() == FILENODE_TYPE::NODE_DIRECTORY) {
            ImGui::ImageButton((void*)m_fileNodeTextureMap[".folder"], btnSize);
            // �{�^�����_�u���N���b�N�����Ƃ�
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                PushDirButton(child);
            }
        }
        // �t�@�C���{�^���������ꂽ�Ƃ�
        else {
            // �t�@�C���̊g���q�ɂ���ĕ\������e�N�X�`���{�^����ς���
            std::string extName = GetFileExtension(child->GetFileName());
            if (ImGui::ImageButton((void*)m_fileNodeTextureMap[extName], btnSize)) {
                PushFileButton(child);
            }
            // �t�@�C���{�^�����_�u���N���b�N������
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                // �V�[���t�@�C���Ȃ�
                if (m_selectNode->GetFileName() == "Scenes" && extName == ".json") {
                    PushSceneFileButton(child);
                }
            }
        }
        // �t�@�C��������������ꍇ�ȗ�
        std::string fileName = CutFileName(child->GetFileName(),btnSize.x);
        // �t�@�C������\��
        ImGui::Text(fileName.c_str());
        ImGui::EndGroup();
        float lastButtonPos = ImGui::GetItemRectMax().x;
        float nextButtonPos = lastButtonPos + style.ItemSpacing.x + btnSize.x;
        // ���̃{�^���̈ʒu���E�B���h�E����͂ݏo���Ȃ��Ȃ瓯���s�ɔz�u
        if (n + 1 < btnCount && nextButtonPos < windowVisibleSize)
            ImGui::SameLine(0.0f,20.0f);
        ImGui::PopID();
        n++;
    }
    // �V�[���t�H���_���J���Ă���ꍇ�V�[���ǉ��{�^����\��
    if (m_selectNode->GetFileName() == "Scenes") {
        ImGui::ImageButton((void*)m_plusButtonTexture, btnSize);
        // �{�^�����_�u���N���b�N������
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            ImGui::OpenPopup(CREATE_SCENE_POPUP_NAME);
        }
    }
    // �X�N���v�g�t�H���_���J���Ă���ꍇ�X�N���v�g�ǉ��{�^����\��
    else if (m_selectNode->GetFileName() == "Scripts") {
        ImGui::ImageButton((void*)m_plusButtonTexture, btnSize);
        // �{�^�����_�u���N���b�N������
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            ImGui::OpenPopup(CREATE_SCRIPT_POPUP_NAME);
        }
    }
    // �X�^�C�������Z�b�g
    style = ImGuiStyle();
    ShowCreateScenePopup();
    ShowCreateScriptPopup();

}

/// <summary>
/// �t�@�C��������������ꍇ�ȗ�����֐�
/// </summary>
/// <param name="fileName">�ȗ�����֐�</param>
/// <param name="maxWidth">�ȗ����钷���̌��E�l</param>
/// <returns>�ȗ������t�@�C����</returns>
std::string ProjectGui::CutFileName(std::string fileName,int maxWidth)
{
    std::string cutName = fileName;
    std::string cutSymbol = "..";
    float cutTextWidth = ImGui::CalcTextSize(cutSymbol.c_str()).x;
    // �t�@�C����������l�̒����ɂȂ�܂ŕ������ȗ�
    while (ImGui::CalcTextSize(cutName.c_str()).x > (maxWidth - cutTextWidth)) {
        cutName.pop_back();
    };
    // �t�@�C�������ȗ����Ă����Ȃ�ȗ��L��������
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
/// �w�肳�ꂽ�f�B���N�g���ɂ���t�H���_��t�@�C����؍\���Ɋi�[����֐�
/// </summary>
/// <param name="fileNode">�i�[����m�[�h</param>
/// <param name="registerID">�i�[����m�[�h��ID</param>
void ProjectGui::FindFile(FileTreeNode* fileNode, int* registerID)
{
    HANDLE fHandle;
    WIN32_FIND_DATA win32fd;

    std::string searchName = fileNode->GetFileFullPath()+"\\*";

    fHandle = FindFirstFile(searchName.c_str(), &win32fd);

    // �t�@�C�����Ȃ��Ȃ�I��
    if (fHandle == INVALID_HANDLE_VALUE) {
        // "'GetLastError() == 3' is 'file not found'"
        return;
    }

    do {
        if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            //�f�B���N�g���ł���
            //���̃f�B���N�g�����̂��̂�\���ꍇ�͏������Ȃ�
            if (win32fd.cFileName[0] == '.') continue;
            auto childNode = fileNode->Insert(*registerID, FILENODE_TYPE::NODE_DIRECTORY, win32fd.cFileName);
            (*registerID)++;
            //�ċA�I�Ƀt�@�C��������
            FindFile(childNode,registerID);
        }
        else {
            //�t�@�C���ł���
            // �g���q��.obj .png .jpg .json�Ȃ�i�[
            std::string fileName = win32fd.cFileName;
            std::string extName = GetFileExtension(fileName);
            if (extName != ".obj" && extName != ".png" && extName != ".jpg" && extName != ".json" && extName != ".lua" && extName != ".hlsl") continue;
            // �m�[�h�ɑ}��
            fileNode->Insert((*registerID), FILENODE_TYPE::NODE_FILE, win32fd.cFileName);
            (*registerID)++;
        }
    } while (FindNextFile(fHandle, &win32fd));

    FindClose(fHandle);
}


/// <summary>
/// �t�@�C���؍\�����X�V����֐�
/// </summary>
/// <param name="fileNode"></param>
void ProjectGui::UpdateFile(FileTreeNode* fileNode)
{

}


/// <summary>
/// �f�B���N�g���{�^�������������̏���
/// </summary>
/// <param name="fileNode"></param>
void ProjectGui::PushDirButton(FileTreeNode* fileNode)
{
    SetFileNode(fileNode);
}


/// <summary>
/// �t�@�C���{�^�������������̏���
/// </summary>
/// <param name="fileNode"></param>
void ProjectGui::PushFileButton(FileTreeNode* fileNode)
{
    auto inspector = MyImGuiManager::GetInstance()->GetImGui<InspectorGui>();
    inspector->SetFileNode(fileNode);
}


/// <summary>
/// �V�[���t�@�C���{�^�������������̏���
/// </summary>
/// <param name="fileNode">�I�������m�[�h�̏��</param>
void ProjectGui::PushSceneFileButton(FileTreeNode* fileNode)
{
    std::string sceneName = GetNoFileExtension(fileNode->GetFileName());
    // �V�[����ǂݍ���
    Manager::GetInstance()->ChangeScene(sceneName);
}


/// <summary>
/// �t�@�C���ēǂݍ��݃{�^�������������̏���
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
/// �V�[�����쐬����|�b�v�A�b�v��\������֐�
/// </summary>
void ProjectGui::ShowCreateScenePopup()
{
    // �V�[�����쐬����|�b�v�A�b�v��\��
    if (ImGui::BeginPopupModal(CREATE_SCENE_POPUP_NAME, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
        ImGui::Separator();
        static char str[128] = "";
        ImGui::InputText(".json", str, IM_ARRAYSIZE(str));
        ImVec2 btnSize = ImVec2(60, 30);
        if (ImGui::Button("OK", btnSize)) {
            std::string fileName = str;
            // �g���q������Ȃ�O��
            if (GetFileExtension(fileName) == ".json") {
                fileName = GetNoFileExtension(fileName);
            }
            // �t�@�C���������͂���Ă��邩�`�F�b�N
            if (fileName == "") {
                MyImGuiManager::GetInstance()->DebugLog("Faild!! This scene name is enpty!!");
            }
            // �����V�[���������邩�`�F�b�N
            else if (m_selectNode->IsExistSameFileName(fileName+".json")) {
                MyImGuiManager::GetInstance()->DebugLog("Faild!! This scene name is already in use!!");
            }
            else {
                // �V�[�����쐬����
                if (Manager::GetInstance()->CreateScene(fileName)) {
                    // �X�N���v�g���쐬�����Ȃ�t�@�C���؍\���ɒǉ�
                    m_selectNode->Insert(m_registerID, FILENODE_TYPE::NODE_FILE, fileName + ".json");
                    m_registerID++;
                    // �����������Ƃ�ʒm
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
/// �X�N���v�g���쐬����|�b�v�A�b�v��\������֐�
/// </summary>
void ProjectGui::ShowCreateScriptPopup()
{
    // �X�N���v�g���쐬����|�b�v�A�b�v��\��
    if (ImGui::BeginPopupModal(CREATE_SCRIPT_POPUP_NAME,NULL,ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
        ImGui::Separator();
        static char str[128] = "";
        ImGui::InputText(".lua", str, IM_ARRAYSIZE(str));
        ImVec2 btnSize = ImVec2(60, 30);
        if (ImGui::Button("OK", btnSize)) {
            std::string fileName = str;
            // �g���q���Ȃ��Ȃ�t������
            if (GetFileExtension(fileName) != ".lua") {
                fileName += ".lua";
            }
            // �t�@�C���������͂���Ă��邩�`�F�b�N
            if (fileName == ".lua") {
                MyImGuiManager::GetInstance()->DebugLog("Faild!! This file name is enpty!!");
            }
            // �����t�@�C���������邩�`�F�b�N
            else if (m_selectNode->IsExistSameFileName(fileName)) {
                MyImGuiManager::GetInstance()->DebugLog("Faild!! This file name is already in use!!");
            }
            else {
                // �X�N���v�g���쐬����
                if (LuaManager::GetInstance()->CreateScriptFile(GetNoFileExtension(fileName))) {
                    // �X�N���v�g���쐬�����Ȃ�t�@�C���؍\���ɒǉ�
                    m_selectNode->Insert(m_registerID, FILENODE_TYPE::NODE_FILE, fileName);
                    m_registerID++;
                    // �����������Ƃ�ʒm
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

