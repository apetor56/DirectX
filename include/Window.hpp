#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "RWin.hpp"
#include <sstream>
#include <optional>
#include <memory>
#include "DXException.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Timer.hpp"
#include "Graphics.hpp"

#define DXWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define DXWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())

class Window {
public:
    Keyboard keyboard;
    Mouse mouse;
    Timer timer;
    
    Window(long width, long height, const char *name);

    ~Window();

    Window(const Window& other) = delete;

    Window& operator=(const Window& other) = delete;

    void setTitle(const std::string& title);

    static std::optional<int> processMessages();

    class Exception : public DXException {
    public:
        Exception(int line, const char *file, HRESULT hr) noexcept;

        const char *what() const noexcept override;

        virtual const char* getType() const noexcept override;

        static std::string translateErrorCode(HRESULT hr) noexcept;

        HRESULT getErrorCode() const noexcept;

        std::string getErrorString() const noexcept;
    
    private:
        HRESULT hr;
    };

    Graphics& gfx();

private:
    long width;
    long height;
    HWND hWnd;
    std::unique_ptr<Graphics> graphics_ptr;

//////////////////////////////////////////////////////////////////////

    /* nestes class for our window class to register */
    class WindowClass {
    public:
        static const char *getName() noexcept;
        static HINSTANCE getInstance() noexcept;

    private:
        static WindowClass windowClass;
        HINSTANCE hInstance;                // it will be handler to out aplication

        WindowClass() noexcept;

        ~WindowClass();

        WindowClass(const WindowClass& other) = delete;

        WindowClass& operator=(const WindowClass& other) = delete;

        static constexpr const char* wndClassName = "some name";
    };

//////////////////////////////////////////////////////////////////////

    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};

#endif // WINDOW_HPP
