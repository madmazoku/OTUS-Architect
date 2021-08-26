// Console Convey's Life Game

#include <windows.h>

#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>
#include <random>

const SHORT WIDTH = 900;
const SHORT HEIGHT = 250;
const DWORD BUFFER_SIZE = ((DWORD)WIDTH) * ((DWORD)HEIGHT);
const WCHAR OCCUPIED_POINT = L'■';
const WCHAR FREE_POINT = L' ';

#include <strsafe.h>
void ErrorExit(LPCTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

DWORD pos_to_offset(int x, int y) {
	if (x < 0)
		x += WIDTH;
	if (x >= WIDTH)
		x -= WIDTH;

	if (y < 0)
		y += HEIGHT;
	if (y >= HEIGHT)
		y -= HEIGHT;

	return y * WIDTH + x;
}

void do_life_step(WCHAR first_memory_buffer[], WCHAR second_memory_buffer[], DWORD static_indexes[]) {
	for (DWORD idx = 0; idx < BUFFER_SIZE; ++idx, ++second_memory_buffer) {
		int neighbors = 0;
		for (DWORD n = 0; n < 8; ++n, ++static_indexes)
			neighbors += first_memory_buffer[*static_indexes] == OCCUPIED_POINT ? 1 : 0;
		bool alive = first_memory_buffer[idx] == OCCUPIED_POINT;
		*second_memory_buffer = neighbors == 3 || (alive && neighbors == 2) ? OCCUPIED_POINT : FREE_POINT;
	}
}

void fill_random_state(WCHAR memory_buffer[]) {
	std::random_device rd;
	std::default_random_engine re(rd());
	std::uniform_int_distribution<int> uniform_dist(1, 10);

	for (DWORD idx = 0; idx < BUFFER_SIZE; ++idx) {
		if (uniform_dist(re) > 1)
			memory_buffer[idx] = FREE_POINT;
		else
			memory_buffer[idx] = OCCUPIED_POINT;
	}
}

int main()
{
	::SetConsoleTitle(L"Convey Life: N - new table; Q - quit");

	HANDLE input_buffer_handle = ::GetStdHandle(STD_INPUT_HANDLE);
	if (input_buffer_handle == INVALID_HANDLE_VALUE)
		ErrorExit(L"::GetStdHandle");

	HANDLE first_buffer_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (first_buffer_handle == INVALID_HANDLE_VALUE)
		ErrorExit(L"::GetStdHandle");

	CONSOLE_FONT_INFOEX screen_font_info;
	::ZeroMemory(&screen_font_info, sizeof(CONSOLE_FONT_INFOEX));
	screen_font_info.cbSize = sizeof(CONSOLE_FONT_INFOEX);

	if (!::GetCurrentConsoleFontEx(first_buffer_handle, FALSE, &screen_font_info))
		ErrorExit(L"::GetCurrentConsoleFontEx");

	screen_font_info.nFont = 0;
	screen_font_info.dwFontSize = { 5, 5 };
	screen_font_info.FontFamily = 0x36;
	screen_font_info.FontWeight = 400;
	wcscpy_s(screen_font_info.FaceName,  L"Courier New");

	if (!::SetCurrentConsoleFontEx(first_buffer_handle, FALSE, &screen_font_info))
		ErrorExit(L"::SetCurrentConsoleFontEx");

	CONSOLE_SCREEN_BUFFER_INFOEX screen_buffer_info;
	::ZeroMemory(&screen_buffer_info, sizeof(CONSOLE_SCREEN_BUFFER_INFOEX));
	screen_buffer_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

	if (!::GetConsoleScreenBufferInfoEx(first_buffer_handle, &screen_buffer_info))
		ErrorExit(L"::GetConsoleScreenBufferInfoEx");

	screen_buffer_info.dwSize = { WIDTH, HEIGHT };
	screen_buffer_info.dwMaximumWindowSize = { WIDTH, HEIGHT };
	screen_buffer_info.dwCursorPosition = { 0, 0 };
	screen_buffer_info.srWindow = { 0, 0, WIDTH, HEIGHT };

	if (!::SetConsoleScreenBufferInfoEx(first_buffer_handle, &screen_buffer_info))
		ErrorExit(L"::SetConsoleScreenBufferInfoEx");

	HANDLE second_buffer_handle = ::CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	if(second_buffer_handle == INVALID_HANDLE_VALUE)
		ErrorExit(L"::CreateConsoleScreenBuffer");

	if (!::SetConsoleScreenBufferInfoEx(second_buffer_handle, &screen_buffer_info))
		ErrorExit(L"::SetConsoleScreenBufferInfoEx");

	DWORD* static_indexes = new DWORD[BUFFER_SIZE * 8];
	WCHAR* first_memory_buffer = new WCHAR[BUFFER_SIZE];
	WCHAR* second_memory_buffer = new WCHAR[BUFFER_SIZE];
	WCHAR* third_memory_buffer = new WCHAR[BUFFER_SIZE];

	DWORD* static_indexes_it = static_indexes;
	for (int y = 0; y < HEIGHT; ++y)
		for (int x = 0; x < WIDTH; ++x)
			for (int yy = y - 1; yy <= y + 1; ++yy)
				for (int xx = x - 1; xx <= x + 1; ++xx)
					if (x != xx || y != yy)
						*(static_indexes_it++) = pos_to_offset(xx, yy);

	::ZeroMemory(first_memory_buffer, BUFFER_SIZE * sizeof(WCHAR));
	::ZeroMemory(second_memory_buffer, BUFFER_SIZE * sizeof(WCHAR));
	::ZeroMemory(third_memory_buffer, BUFFER_SIZE * sizeof(WCHAR));

	fill_random_state(second_memory_buffer);
	do_life_step(second_memory_buffer, first_memory_buffer, static_indexes);

	if (!::WriteConsole(first_buffer_handle, first_memory_buffer, BUFFER_SIZE, NULL, NULL))
		ErrorExit(L"::WriteConsole");

	bool active = true;
	while (active) {

		std::swap(first_buffer_handle, second_buffer_handle);
		std::swap(first_memory_buffer, third_memory_buffer); // 1, 2, 3 -> 3, 2, 1
		std::swap(second_memory_buffer, third_memory_buffer); // 3, 2, 1 -> 3, 1, 2

		do_life_step(second_memory_buffer, first_memory_buffer, static_indexes);

		if (wmemcmp(first_memory_buffer, third_memory_buffer, BUFFER_SIZE) == 0) {
			fill_random_state(second_memory_buffer);
			do_life_step(second_memory_buffer, first_memory_buffer, static_indexes);
		}

		if (!::SetConsoleScreenBufferInfoEx(first_buffer_handle, &screen_buffer_info))
			ErrorExit(L"::SetConsoleScreenBufferInfoEx");

		if (!::WriteConsole(first_buffer_handle, first_memory_buffer, BUFFER_SIZE, NULL, NULL))
			ErrorExit(L"::WriteConsole");

		INPUT_RECORD input_record;
		::ZeroMemory(&input_record, sizeof(INPUT_RECORD));
		DWORD events_read = 0;

		while (active) {
			if (!::PeekConsoleInput(input_buffer_handle, &input_record, 1, &events_read))
				ErrorExit(L"PeekConsoleInput");
			if (events_read == 0)
				break;
			::ReadConsoleInput(input_buffer_handle, &input_record, 1, &events_read);
			if (input_record.EventType == KEY_EVENT && input_record.Event.KeyEvent.bKeyDown) {
				switch (input_record.Event.KeyEvent.wVirtualKeyCode) {
				case 0x51: // Q
					active = false;
					break;
				case 0x4E: // N
					fill_random_state(second_memory_buffer);
					do_life_step(second_memory_buffer, first_memory_buffer, static_indexes);
					break;
				}
			}
		}

		if (!::SetConsoleActiveScreenBuffer(first_buffer_handle))
			ErrorExit(L"::SetConsoleActiveScreenBuffer");
	}

	delete[] static_indexes;
	delete[] first_memory_buffer;
	delete[] second_memory_buffer;
}

