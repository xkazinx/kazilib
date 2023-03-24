#include "pch.h"
#include "os.h"

namespace kazin::os
{
void ShowDesktop()
{
	// #todo
	/*key::Down(VK_LWIN);
	key::Down(0x44); // 'D'
	key::Up(VK_LWIN);
	key::Up(0x44); // 'D'*/
}

void SetClipboard(const string & data)
{
	const char *  output = data.c_str();
	const size_t  len    = strlen(output) + 1;
	const HGLOBAL hMem   = GlobalAlloc(GMEM_MOVEABLE, len);

	memcpy(GlobalLock(hMem), output, len);
	GlobalUnlock(hMem);
	OpenClipboard(nullptr);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
}

bool LogError(const string& identifier)
{
	if (auto id = ::GetLastError();
		id != 0)
	{
		LPSTR messageBuffer = nullptr;

		const size_t size = FormatMessageA
		(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			id,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&messageBuffer),
			0,
			nullptr
		);

		PrintError("{} Win32 ({}):{}",
			identifier, id, string{ messageBuffer, size });

		LocalFree(messageBuffer);
		return true;
	}

	return false;
}

void ResizeWindow(const HWND hwnd, int width, int height)
{
	RECT rClient;
	GetClientRect(hwnd, &rClient);
	if (width == rClient.right - rClient.left &&
		height == rClient.bottom - rClient.top)
	{
		return;
	}

	RECT rWindow;
	GetWindowRect(hwnd, &rWindow);
	width += (rWindow.right - rWindow.left) - rClient.right;
	height += (rWindow.bottom - rWindow.top) - rClient.bottom;

	// SetWindowPos will hang if this ever happens.
	//assert(GetCurrentThreadId() == GetWindowThreadProcessId(hwnd, NULL));

	SetWindowPos(hwnd, nullptr, rWindow.left, rWindow.top, width, height, SWP_NOMOVE);
}
}
