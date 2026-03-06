#include "app_2048.h"

std::wstring const app_2048::s_class_name{ L"app_2048_class" };

app_2048::app_2048(HINSTANCE instance) : m_instance{ instance }, m_main{} {
    m_bg_brush = CreateSolidBrush(RGB(30, 50, 90));
    m_rect_brush = CreateSolidBrush(RGB(170, 70, 80));
    register_class();
    m_main = create_window();
}

app_2048::~app_2048() {
    DeleteObject(m_bg_brush);
    DeleteObject(m_rect_brush);
}

bool app_2048::register_class() {
    WNDCLASSEXW desc{ .cbSize = sizeof(WNDCLASSEXW) };
    if (GetClassInfoExW(m_instance, s_class_name.c_str(), &desc) != 0) return true;

    desc.lpfnWndProc = window_proc_static;
    desc.hInstance = m_instance;
    desc.hCursor = LoadCursorW(nullptr, L"IDC_ARROW");
    desc.hbrBackground = m_bg_brush;
    desc.lpszClassName = s_class_name.c_str();
    return RegisterClassExW(&desc) != 0;
}

HWND app_2048::create_window() {
    // Obliczamy wymiary okna tak, aby obszar klienta (œrodek) mia³ dok³adnie 800x600
    RECT rc = { 0, 0, 800, 600 };
    AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

    return CreateWindowExW(
        0, s_class_name.c_str(), L"Not WM_PAINT",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        nullptr, nullptr, m_instance, this);
}

LRESULT app_2048::window_proc_static(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
    app_2048* app = nullptr;
    if (message == WM_NCCREATE) {
        auto p = reinterpret_cast<LPCREATESTRUCTW>(lparam);
        app = static_cast<app_2048*>(p->lpCreateParams);
        SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
    }
    else {
        app = reinterpret_cast<app_2048*>(GetWindowLongPtrW(window, GWLP_USERDATA));
    }
    return (app) ? app->window_proc(window, message, wparam, lparam)
        : DefWindowProcW(window, message, wparam, lparam);
}

LRESULT app_2048::window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
    case WM_LBUTTONDOWN: {
        int x = (short)LOWORD(lparam);
        int y = (short)HIWORD(lparam);

        // Tworzenie prostok¹ta jako okna potomnego klasy STATIC
        HWND hRect = CreateWindowExW(0, L"STATIC", nullptr,
            WS_CHILD | WS_VISIBLE,
            x, y, 100, 100, // Na razie sta³y rozmiar 100x100
            window, nullptr, m_instance, nullptr);

        m_rects.push_back(hRect);
        return 0;
    }

    case WM_CTLCOLORSTATIC: {
        // Zwracamy pêdzel, aby prostok¹ty (STATIC) mia³y odpowiedni kolor
        return (LRESULT)m_rect_brush;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(window, message, wparam, lparam);
}

int app_2048::run(int show_command) {
    ShowWindow(m_main, show_command);
    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}