#pragma once

#include "PipeLine.h"

#include <iostream>

#include <Windows.h>

namespace KNVM {
	namespace UIProc {
		extern LRESULT CALLBACK WndProc(
			_In_ HWND   hwnd,
			_In_ UINT   uMsg,
			_In_ WPARAM wParam,
			_In_ LPARAM lParam
		);
	}

	class UI {
	private:
		static HINSTANCE hUIinst;
		static HWND hShell;
		static HWND hCmd;
		static WNDPROC wCmdProc;
		static bool shell;
		
	private:
		static PipeLine pipe;

	public:
		UI(bool shell = false) {
			WNDCLASSEX wnd;

			ZeroMemory(&wnd, sizeof(WNDCLASSEX));
			wnd.cbSize = sizeof(WNDCLASSEX);
			wnd.style = CS_HREDRAW | CS_VREDRAW;
			wnd.hbrBackground = shell ? (HBRUSH)GetStockObject(BLACK_BRUSH) : (HBRUSH)GetStockObject(WHITE_BRUSH);
			wnd.lpfnWndProc = UIProc::WndProc;
			wnd.hInstance = hUIinst;
			wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
			wnd.lpszClassName = "KNVM Shell";

			if (!RegisterClassEx(&wnd)) {
				exit(-1);
			}

			hShell = CreateWindowA("KNVM Shell", "KNVM Shell", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hUIinst, NULL);
			if (!hShell) {
				exit(-1);
			}

			ShowWindow(hShell, SW_SHOW);
			UpdateWindow(hShell);

			this->shell = shell;
			if (shell) {
				RECT rect;
				GetClientRect(hShell, &rect);

				hCmd = CreateWindowA("edit", "CmdBox", WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hShell, NULL, hUIinst, NULL);
				if (!hCmd) {
					exit(-1);
				}

				HDC hdcCmd = GetDC(hCmd);
				SendMessage(hCmd, WM_CTLCOLOREDIT, (WPARAM)hdcCmd, (LPARAM)hCmd);

				ShowWindow(hCmd, SW_SHOW);
				UpdateWindow(hCmd);
			}
		
			MSG msg;
			while (GetMessage(&msg, 0, 0, 0)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		~UI() {
			if(hShell)
				CloseHandle(hShell);
			if(hUIinst)
				CloseHandle(hUIinst);
		}

		static auto gethUI() { return hUIinst; }
		static auto gethShell() { return hShell; }
		static auto gethCmd() { return hCmd; }
		static auto isShell() { return shell; }
	};
}