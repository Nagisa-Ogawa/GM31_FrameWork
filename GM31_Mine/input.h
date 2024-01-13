#pragma once

#define KEY_A ('A')
#define KEY_D ('D')
#define KEY_E ('E')
#define KEY_Q ('Q')
#define KEY_S ('S')
#define KEY_W ('W')

class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger(BYTE KeyCode );
	static POINT GetClientMousePos();

};
