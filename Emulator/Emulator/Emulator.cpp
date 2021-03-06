#include "stdafx.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
EGLDisplay display;
EGLSurface eglSurface;
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Learn to Program Windows",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}


	display = eglGetDisplay(GetDC(hwnd));
	if (display == EGL_NO_DISPLAY) {
		MessageBox(hwnd, L"No display.", L"DEBUG", MB_HELP);
	}

	EGLint majorVersion, minorVersion;
	if (! eglInitialize(display, &majorVersion, &minorVersion)) {
		MessageBox(hwnd, L"Could not init.", L"DEBUG", MB_HELP);
	}

	eglBindAPI(EGL_OPENGL_ES_API);

	EGLint configAttributes[] =
	{
		EGL_RENDERABLE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_NONE
	};

	EGLint numConfigs;
	EGLConfig windowConfig;
	
	MessageBox(hwnd, L"Here", L"DEBUG", MB_HELP);

	if (! eglChooseConfig(display, configAttributes, &windowConfig, 1, &numConfigs)) {
		MessageBox(hwnd, L"Could not determine config", L"DEBUG", MB_HELP);
	}
	
	EGLint surfaceAttributes[] = { EGL_NONE };
	eglSurface = eglCreateWindowSurface(display, windowConfig, hwnd, surfaceAttributes);

	if (eglSurface == EGL_NO_SURFACE) {
		MessageBox(hwnd, L"No surface", L"DEBUG", MB_HELP);
	}

	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
	EGLContext eglContext = eglCreateContext(display, windowConfig, NULL, contextAttribs);

	if (eglContext == EGL_NO_CONTEXT) {
		MessageBox(hwnd, L"No Context", L"DEBUG", MB_HELP);
	}
	
	if (! eglMakeCurrent(display, eglSurface, eglSurface, eglContext)) {
		MessageBox(hwnd, L"No Context", L"DEBUG", MB_HELP);
	}

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		display = NULL;
		eglSurface = NULL;
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		glClear(GL_COLOR_BUFFER_BIT);
		eglSwapBuffers(display, eglSurface);

		EndPaint(hwnd, &ps);

	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
