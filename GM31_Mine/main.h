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

// 画面サイズ
#define SCREEN_WIDTH	(1540)
#define SCREEN_HEIGHT	(845)
// ゲーム画面が映る画面サイズ
#define GAMEWINDOW_WIDTH	(SCREEN_WIDTH*1.0f)
#define GAMEWINDOW_HEIGHT	(SCREEN_HEIGHT*1.0f)


HWND GetWindow();


