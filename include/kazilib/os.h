#pragma once

#ifdef KAZIN_WINDOWS
#include <windows.h> 
#undef min
#undef max
#endif

namespace kazin::os
{
using Path = std::filesystem::path;

extern void SetClipboard(const string & data);
extern void ShowDesktop();

extern bool LogError(const string & identifier = "");

extern void ResizeWindow(const HWND hwnd, int width, int height);
}
