#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "app_2048.h"

int WINAPI wWinMain(HINSTANCE instance,
    HINSTANCE /*hPrevInstance*/,
    LPWSTR /*lpCmdLine*/,
    int show_command)
{
    // Tworzy instancjê aplikacji, rejestruje klasê i tworzy g³ówne okno
    app_2048 app{ instance };

    // Uruchamia pêtlê komunikatów
    return app.run(show_command);
}