#pragma once
#include <Windows.h>
#include <string>

// �t�@�C���I���_�C�A���O���o���A�I�����ꂽ�t�@�C���̃p�X��Ԃ��֐�
bool OpneFileDialog(HWND hwnd, std::string dialogName, std::string openFilePath, const char* filter, std::string* selectFilePath);
