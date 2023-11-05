#include "Window.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, 2000, NULL);
		break;

	case WM_CLOSE:
		KillTimer(hwnd, 1);
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);


		RECT rcClient;
		GetClientRect(hwnd, &rcClient);// 창 크기 구하기
		FillRect(hdc, &rcClient, (HBRUSH)(COLOR_WINDOW + 1)); // 전부 기본색으로 채우기

		DrawShape::GetInst()->Run(hwnd, hdc);

		int score = DrawShape::GetInst()->ReturnScore();
				
		wchar_t scoreStr[16];
		_itow_s(score, scoreStr, 10);

		wchar_t scoreText[32];
		swprintf_s(scoreText, 31, L"Score : %s", scoreStr);

		wchar_t blockText[2];

		switch (DrawShape::GetInst()->ReturnSavedBlockType())
		{
		case BlockType::Box:
			{
			const wchar_t* tmp = L"o";
			swprintf_s(blockText, 2, tmp); }
			break;

		case BlockType::L:
			{
			const wchar_t* tmp = L"L";
			swprintf_s(blockText, 2, tmp); }
			break;

		case BlockType::ReverseL:
			{
			const wchar_t* tmp = L"J";
			swprintf_s(blockText, 2, tmp); }
			break;

		case BlockType::ReverseZ:
			{
			const wchar_t* tmp = L"S";
			swprintf_s(blockText, 2, tmp); }
			break;

		case BlockType::Stick:
			{
			const wchar_t* tmp = L"I";
			swprintf_s(blockText, 2, tmp); }
			break;

		case BlockType::T:
			{
			const wchar_t* tmp = L"T";
			swprintf_s(blockText, 2, tmp); }
			break;

		case BlockType::Z:
			{
			const wchar_t* tmp = L"Z";
			swprintf_s(blockText, 2, tmp); }
			break;
		}

		HFONT hFont = CreateFontW(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial");
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

		TextOutW(hdc, 25, 25, (LPCWSTR)scoreText, wcslen(scoreText));
		if (DrawShape::GetInst()->ReturnSavedBlockType() != BlockType::none)
		{
			TextOutW(hdc, 300, 25, (LPCWSTR)blockText, wcslen(blockText));
		}

		SelectObject(hdc, hOldFont);
		DeleteObject(hFont);

		EndPaint(hwnd, &ps);
	}
	break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (!DrawShape::GetInst()->CurBlockDownCheck())
			{
				DrawShape::GetInst()->MoveCurBlock(3);
			}
			InvalidateRect(hwnd, NULL, true);
			break;

		case 2:
			DrawShape::GetInst()->ChangeCurBlock();
			DrawShape::GetInst()->LineCheck();
			break;
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RIGHT:
			std::cout << "RightArrow" << std::endl;
			if (!DrawShape::GetInst()->CurBlockRightCheck())
			{
				DrawShape::GetInst()->MoveCurBlock(2);
			}
			InvalidateRect(hwnd, NULL, true);
			break;

		case VK_LEFT:
			std::cout << "LeftArrow" << std::endl;
			if (!DrawShape::GetInst()->CurBlockLeftCheck())
			{
				DrawShape::GetInst()->MoveCurBlock(1);
			}
			InvalidateRect(hwnd, NULL, true);
			break;

		case VK_DOWN:
			std::cout << "DownArrow" << std::endl;
			if (!DrawShape::GetInst()->CurBlockDownCheck())
			{
				DrawShape::GetInst()->MoveCurBlock(3);
			}
			InvalidateRect(hwnd, NULL, true);
			break;

		case VK_SPACE:
			std::cout << "Space" << std::endl;
			DrawShape::GetInst()->DownToBottom();
			InvalidateRect(hwnd, NULL, true);
			break;

		case 0x5A:
			std::cout << "Z" << std::endl;
			DrawShape::GetInst()->BlockSpeen(SpeenDir::left);
			break;

		case 0x58:
			std::cout << "X" << std::endl;
			DrawShape::GetInst()->BlockSpeen(SpeenDir::right);
			break;

		case 0x43:
			std::cout << "C" << std::endl;
			DrawShape::GetInst()->BlockChange();
			break;
		}
	break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Window::Window()
	: m_hInstance(GetModuleHandle(nullptr))
{
	DrawShape::GetInst()->Init(m_hWnd);

	const wchar_t* CLASS_NAME = L"Window Class";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WindowProc;

	RegisterClass(&wndClass);

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	int width = 325;
	int height = 650;

	RECT rect;
	rect.left = 250;
	rect.top = 250;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	AdjustWindowRect(&rect, style, false);

	m_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"TITLE",
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	ShowWindow(m_hWnd, SW_SHOW);
}

Window::~Window()
{
	const wchar_t* CLASS_NAME = L"Window Class";

	UnregisterClass(CLASS_NAME, m_hInstance);
}

bool Window::ProcessMessages()
{
	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}


