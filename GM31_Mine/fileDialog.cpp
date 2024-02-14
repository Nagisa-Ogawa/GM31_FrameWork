#include "fileDialog.h"

/// <summary>
/// ファイルダイアログを開き選択されたファイルパスを取得する関数
/// 取得するパスはこのアプリケーションからの相対パス
/// </summary>
/// <param name="hwnd">ウィンドウハンドル</param>
/// <param name="dialogName">ダイアログのタイトル</param>
/// <param name="openFilePath">開くフォルダの位置</param>
/// <param name="filter">どの種類のファイルを選ぶかのフィルター</param>
/// <param name="selectFilePath">選択したファイルのパス</param>
/// <returns>ファイルの選択をしたかどうか</returns>
bool OpneFileDialog(HWND hwnd, std::string dialogName, std::string openFilePath, const char* filter, std::string* selectFilePath)
{
    OPENFILENAME ofn{};
    TCHAR szPath[MAX_PATH]{};
    TCHAR szFile[MAX_PATH]{};

    // このアプリケーションまでのフルパスを取得
    if (szPath[0] == TEXT('\0')) {
        GetCurrentDirectory(MAX_PATH, szPath);
    }
    // フルパスに表示したいフォルダまでのパスを足す
    openFilePath = "\\" + openFilePath;
    openFilePath = szPath + openFilePath;

    if (ofn.lStructSize == 0) {
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hwnd;
        ofn.lpstrInitialDir = openFilePath.c_str();       // フォルダ位置
        ofn.lpstrFile = szFile;       // 選択ファイルパス
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = TEXT(filter);
        ofn.lpstrTitle = dialogName.c_str();
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    }
    // ファイルダイアログ表示
    if (GetOpenFileName(&ofn)) {
        // 選択したファイルを受け取る
        *selectFilePath = ofn.lpstrFile;
        // フルパスをこのアプリケーションからの相対パスに変換
        std::string szStr = szPath;
        // \\を取り除く為に+1
        int pathNum = szStr.size() + 1;
        int fullPathNum = selectFilePath->size();
        *selectFilePath = selectFilePath->substr(pathNum, fullPathNum - pathNum);
        return true;
    }
    return false;
}
