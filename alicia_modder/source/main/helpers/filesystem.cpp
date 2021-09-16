//
// Created by maros on 16/09/2021.
//

#include "filesystem.hpp"

#include <windows.h>
#include <shlobj_core.h>

std::string filesystem::helper::UserSelectFile(const char *title,
                                               const char *filter,
                                               int flags) {
    char path[MAX_PATH];
    ZeroMemory(path, MAX_PATH);

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = path;
    ofn.lpstrTitle = title;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ENABLESIZING | OFN_NOCHANGEDIR | flags;
    ofn.lpstrDefExt = "";

    GetOpenFileName(&ofn);

    return path;
}

std::string filesystem::helper::UserSelectFolder(const char *title, int flags) {
    char path[MAX_PATH];
    ZeroMemory(path, MAX_PATH);

    BROWSEINFO bi{};
    bi.pidlRoot = NULL;
    bi.pszDisplayName = path;
    bi.lpszTitle = title;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | flags;
    bi.lpfn = NULL;
    bi.lParam = 0;
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl != nullptr)
        SHGetPathFromIDList(pidl, path);

    return path;
}

void filesystem::helper::UserMessage(const char *title, const char *desc) {
    ::MessageBox(NULL, desc, title, MB_OK);
}
