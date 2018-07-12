#include "UI.h"

namespace KNVM {
	// Static Variable Init
	HINSTANCE UI::hUIinst = GetModuleHandle(NULL);
	HWND UI::hShell = NULL;

	namespace UIProc {
		void keydown(HWND hwnd, WPARAM wParam, LPARAM lParam);

		typedef struct Cursor {
			DWORD x;
			DWORD y;
			DWORD fontsize;
		} Cursor;

		typedef struct Window {
			Cursor cursor;
			DWORD width;
			DWORD height;
		} Window;

		Window window = { { 0, 0, 20 }, 0, 0 };

		LRESULT CALLBACK WndProc(
			_In_ HWND   hwnd,
			_In_ UINT   uMsg,
			_In_ WPARAM wParam,
			_In_ LPARAM lParam
		) {
			RECT rect;
			switch (uMsg) {
			case WM_CREATE:
				GetClientRect(hwnd, &rect);
				window.width = rect.right - rect.left;
				window.height = rect.bottom - rect.top;
				return 0;
			case WM_SIZE: // Resize
				GetClientRect(hwnd, &rect);
				window.width = rect.right - rect.left;
				window.height = rect.bottom - rect.top;
				return 0;
			case WM_PAINT:
				return 0;
			case WM_KEYDOWN:
				keydown(hwnd, wParam, lParam);
				return 0;
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			default:
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
		}

		void keydown(HWND hwnd, WPARAM wParam, LPARAM lParam) {
			if ((wParam >= '0' && wParam <= '9')
				|| (wParam >= 'A' && wParam <= 'Z')
			) {
				PAINTSTRUCT paint;
				HDC hdc = GetDC(hwnd);

				HFONT hFont = CreateFont(window.cursor.fontsize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "±¼¸²");
				HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

				if (window.cursor.x + window.cursor.fontsize * 2 > window.width) {
					window.cursor.x = 0;
					window.cursor.y += window.cursor.fontsize;
				}
				else {
					window.cursor.x += window.cursor.fontsize;
				}
				TextOut(hdc, window.cursor.x, window.cursor.y, (LPCSTR)&wParam, 1);

				SelectObject(hdc, hOldFont);
				DeleteObject(hFont);

				return;
			}

			switch (wParam) {
			case VK_RETURN:
				window.cursor.x = 0;
				window.cursor.y += window.cursor.fontsize;
				break;
			case VK_RIGHT:
			case VK_SPACE:
				if (window.cursor.x + window.cursor.fontsize * 2 > window.width) {
					window.cursor.x = 0;
					window.cursor.y += window.cursor.fontsize;
				}
				else {
					window.cursor.x += window.cursor.fontsize;
				}
				break;
			case VK_LEFT:
				if (window.cursor.x - window.cursor.fontsize * 2 < 0) {
					window.cursor.x = 0;
					window.cursor.y -= window.cursor.fontsize;
				}
				else {
					window.cursor.x -= window.cursor.fontsize;
				}
				break;
			case VK_UP:
				if (window.cursor.y - window.cursor.fontsize * 2 < 0)
					window.cursor.y = 0;
				else
					window.cursor.y -= window.cursor.fontsize;
				break;
			case VK_DOWN:
				if (window.cursor.y - window.cursor.fontsize * 2 < 0)
					window.cursor.y = 0;
				else
					window.cursor.y -= window.cursor.fontsize;
				break;
			case VK_BACK:
				break;
			case VK_RBUTTON:
				break;
			}
		}
	}
}