#pragma once
#include <Windows.h>
#include <string>

// ファイル選択ダイアログを出し、選択されたファイルのパスを返す関数
bool OpneFileDialog(HWND hwnd, std::string dialogName, std::string openFilePath, const char* filter, std::string* selectFilePath);
