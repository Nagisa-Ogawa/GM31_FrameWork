#pragma once

// Luaバインド用キー入力定数
#define KEY_A ('A')
#define KEY_D ('D')
#define KEY_E ('E')
#define KEY_F ('F')
#define KEY_Q ('Q')
#define KEY_S ('S')
#define KEY_W ('W')
#define KEY_RETURN (0x0D)
#define KEY_SHIFT (0x10)
#define KEY_SPACE (0x20)
#define KEY_LEFT  (0x25)
#define KEY_UP    (0x26)
#define KEY_RIGHT (0x27)
#define KEY_DOWN  (0x28)


//-----------------------------------------------------
// ゲーム内入力用クラス
//------------------------------------------------------
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
