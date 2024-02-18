
#include "main.h"
#include "input.h"
#include "MyImGuiManager.h"


BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];


void Input::Init()
{

	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );
	SetKeyboardState(m_KeyState);
}

void Input::Uninit()
{


}

void Input::Update()
{

	memcpy( m_OldKeyState, m_KeyState, 256 );

	GetKeyboardState( m_KeyState );

}

bool Input::GetKeyPress(BYTE KeyCode)
{
	if (MyImGuiManager::GetInstance()->GetFocusWindow() != nullptr &&
		MyImGuiManager::GetInstance()->GetFocusWindow() == ImGui::FindWindowByName("Game")) {
		return (m_KeyState[KeyCode] & 0x80);
	}
	return 0;
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	if (MyImGuiManager::GetInstance()->GetFocusWindow() != nullptr &&
		MyImGuiManager::GetInstance()->GetFocusWindow() == ImGui::FindWindowByName("Game")) {
		return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
	}
	return 0;
}

POINT Input::GetClientMousePos()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(GetWindow(), &mousePos);
	return mousePos;
}
