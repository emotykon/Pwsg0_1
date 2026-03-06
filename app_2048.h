#pragma once
#include <windows.h>
#include <string>
#include <vector>

class app_2048 {
private:
    HINSTANCE m_instance;
    HWND m_main;
    HBRUSH m_bg_brush;      // Kolor t³a okna (30, 50, 90)
    HBRUSH m_rect_brush;    // Kolor prostok¹tów (170, 70, 80)
    std::vector<HWND> m_rects; // Przechowywanie uchwytów do prostok¹tów

    bool register_class();
    HWND create_window();

    static std::wstring const s_class_name;
    static LRESULT CALLBACK window_proc_static(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
    LRESULT window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

public:
    app_2048(HINSTANCE instance);
    ~app_2048();
    int run(int show_command);
};