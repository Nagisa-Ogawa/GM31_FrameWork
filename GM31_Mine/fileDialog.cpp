#include "fileDialog.h"

/// <summary>
/// �t�@�C���_�C�A���O���J���I�����ꂽ�t�@�C���p�X���擾����֐�
/// �擾����p�X�͂��̃A�v���P�[�V��������̑��΃p�X
/// </summary>
/// <param name="hwnd">�E�B���h�E�n���h��</param>
/// <param name="dialogName">�_�C�A���O�̃^�C�g��</param>
/// <param name="openFilePath">�J���t�H���_�̈ʒu</param>
/// <param name="filter">�ǂ̎�ނ̃t�@�C����I�Ԃ��̃t�B���^�[</param>
/// <param name="selectFilePath">�I�������t�@�C���̃p�X</param>
/// <returns>�t�@�C���̑I�����������ǂ���</returns>
bool OpneFileDialog(HWND hwnd, std::string dialogName, std::string openFilePath, const char* filter, std::string* selectFilePath)
{
    OPENFILENAME ofn{};
    TCHAR szPath[MAX_PATH]{};
    TCHAR szFile[MAX_PATH]{};

    // ���̃A�v���P�[�V�����܂ł̃t���p�X���擾
    if (szPath[0] == TEXT('\0')) {
        GetCurrentDirectory(MAX_PATH, szPath);
    }
    // �t���p�X�ɕ\���������t�H���_�܂ł̃p�X�𑫂�
    openFilePath = "\\" + openFilePath;
    openFilePath = szPath + openFilePath;

    if (ofn.lStructSize == 0) {
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hwnd;
        ofn.lpstrInitialDir = openFilePath.c_str();       // �t�H���_�ʒu
        ofn.lpstrFile = szFile;       // �I���t�@�C���p�X
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = TEXT(filter);
        ofn.lpstrTitle = dialogName.c_str();
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    }
    // �t�@�C���_�C�A���O�\��
    if (GetOpenFileName(&ofn)) {
        // �I�������t�@�C�����󂯎��
        *selectFilePath = ofn.lpstrFile;
        // �t���p�X�����̃A�v���P�[�V��������̑��΃p�X�ɕϊ�
        std::string szStr = szPath;
        // \\����菜���ׂ�+1
        int pathNum = szStr.size() + 1;
        int fullPathNum = selectFilePath->size();
        *selectFilePath = selectFilePath->substr(pathNum, fullPathNum - pathNum);
        return true;
    }
    return false;
}
