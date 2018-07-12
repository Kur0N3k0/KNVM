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
		
	private:
		static PipeLine pipe;

	public:
		UI() {
			WNDCLASSEX wnd;

			ZeroMemory(&wnd, sizeof(WNDCLASSEX));
			wnd.cbSize = sizeof(WNDCLASSEX);
			wnd.style = CS_HREDRAW | CS_VREDRAW;
			wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//(BLACK_BRUSH);
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
	};
}