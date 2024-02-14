#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>



#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx11.h>

#pragma warning(pop)



#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")

#pragma comment (lib, "Comdlg32.lib")

// 画面サイズ
#define SCREEN_WIDTH	(1540)
#define SCREEN_HEIGHT	(845)
// ゲーム上でのサイズ
#define GAMESCREEN_WIDTH	(1920)
#define GAMESCREEN_HEIGHT	(1080)

HWND GetWindow();


