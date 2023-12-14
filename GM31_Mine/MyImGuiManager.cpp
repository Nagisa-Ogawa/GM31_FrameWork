
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "MyImGuiManager.h"
#include "MyImGui.h"
#include "ImGui/ImGuizmo.h"
#include "mainMenuBarGui.h"
#include "SceneGui.h"
#include "consoleGui.h"
#include "engineDebugGui.h"
#include "hierarchyGui.h"
#include "inspectorGui.h"
#include "projectGui.h"
#include "gameGui.h"

MyImGuiManager* MyImGuiManager::m_instance = NULL;



MyImGuiManager::MyImGuiManager()
{
    // �Ȃɂ����Ȃ�
}

MyImGuiManager::MyImGuiManager(const MyImGuiManager& manager)
{
	// �C���X�^���X���R�s�[
	m_instance = manager.m_instance;
}

MyImGuiManager& MyImGuiManager::operator=(const MyImGuiManager& manager)
{
	// �C���X�^���X���R�s�[
	m_instance = manager.m_instance;
	return *this;
}


MyImGuiManager::~MyImGuiManager()
{
	// �C���X�^���X�����
	delete m_instance;
}

void MyImGuiManager::Init(HWND hwnd)
{
	// ImGui�̓o�^
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();(void)io;
	// �L�[�{�[�h�ƃR���g���[���[�̓��͂�L����
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 
	// �h�b�L���O�E�B���h�E��L����
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; 
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// io.ConfigWindowsMoveFromTitleBarOnly = TRUE;

	// �J���[�X�^�C����I��
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	
	// imgui�̏������������Ăяo��
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());

	// Gui��ǉ�
	AddImGui<MainMenuBarGui>();
	AddImGui<EngineDebugGui>();
	AddImGui<SceneGui>();
	AddImGui<GameGui>();
	AddImGui<InspectorGui>();
	AddImGui<HierarchyGui>();
	AddImGui<ProjectGui>();
	AddImGui<ConsoleGui>();
}

void MyImGuiManager::Uninit()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void MyImGuiManager::Update()
{

	if (Renderer::GetResizeWidth() != 0 && Renderer::GetResizeHeight() != 0)
	{
		// �����_�[�^�[�Q�b�g���N���A
		Renderer::CleanRenderTarget();
		// �X���b�v�`�F�[���̃o�b�t�@�T�C�Y��ύX���ꂽ�E�B���h�E�̃T�C�Y�ɕύX
		Renderer::GetSwapChain()->ResizeBuffers(0, Renderer::GetResizeWidth(), Renderer::GetResizeHeight(), DXGI_FORMAT_UNKNOWN, 0);
		// �����_�[�^�[�Q�b�g���č쐬
		Renderer::SetRenderTarget(Renderer::GetResizeWidth(), Renderer::GetResizeHeight());
		Renderer::SetResizeWidth(0); Renderer::SetResizeHeight(0);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();


	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());


	for (MyImGui* myImGui : m_myImGuiList)
	{
		myImGui->Update();
	}
}

void MyImGuiManager::Draw()
{
	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}



/// <summary>
/// �C���X�^���X���Ȃ���ΐ������A����΂����Ԃ��֐�
/// </summary>
/// <returns>�C���X�^���X</returns>
MyImGuiManager* MyImGuiManager::GetInstance()
{
	// ���߂Ďg���Ƃ��ɃC���X�^���X�𐶐�
	// ����ȍ~�͐��������C���X�^���X��n��
	if (m_instance == NULL)
	{
		m_instance = new MyImGuiManager();
	}
	return m_instance;

}


/// <summary>
/// ���̃E�B���h�E�Ƀt�H�[�J�X�������Ă���Ȃ�Z�b�g����֐�
/// </summary>
/// <param name="window"></param>
void MyImGuiManager::SetFocusWindow(ImGuiWindow* window)
{
	if (ImGui::IsWindowFocused()) {
		// ���łɃt�H�[�J�X����Ă��邩�`�F�b�N
		if (m_focusWindow==nullptr|| window->ID != m_focusWindow->ID) {
			m_focusWindow = window;
		}
	}
}
