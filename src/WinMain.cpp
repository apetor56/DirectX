#include "App.hpp"

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int cmdShow
) {
    try {
        App{}.go();
    }
    catch(const DXException& e) {
        MessageBox(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch(const std::exception& e) {
        MessageBox(nullptr, e.what(), "Standard exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch(...) {
        MessageBox(nullptr, "No details", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}