#pragma once

#include <Windows.h>
#include "DrawShape.h"
#include <iostream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	Window();
	Window(const Window&) = delete;
	Window& operator =(const Window&) = delete;
	~Window();
	
	bool ProcessMessages();
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

public:
	HWND GetHwnd() { return m_hWnd; }

};

