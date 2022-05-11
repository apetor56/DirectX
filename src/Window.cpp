#include "Window.hpp"
#include <sstream>

Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass() noexcept : hInstance(GetModuleHandle(nullptr)) {
    WNDCLASSEX windowClass = {0};

    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_OWNDC;
    windowClass.lpfnWndProc = HandleMsgSetup;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = nullptr;
    windowClass.hCursor = nullptr;
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = getName();

    RegisterClassEx(&windowClass);
}

Window::WindowClass::~WindowClass() {
    UnregisterClass(wndClassName, getInstance());
}

void Window::setTitle(const std::string& title) {
    if(SetWindowText(hWnd, title.c_str()) == 0) {
        throw DXWND_LAST_EXCEPT();
    }
}

const char* Window::WindowClass::getName() noexcept {
    return wndClassName;
}

HINSTANCE Window::WindowClass::getInstance() noexcept {
    return windowClass.hInstance;
}

Window::Window(long width, long height, const char *name) : width(width), height(height) {
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;

    if(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0) {
        throw DXWND_LAST_EXCEPT();
    }

    hWnd = CreateWindow(
        WindowClass::getName(), name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, WindowClass::getInstance(), this
    );

    if(hWnd == nullptr) {
        throw DXWND_LAST_EXCEPT();
    }

    ShowWindow(hWnd, SW_SHOWDEFAULT);

    // create Graphics object after creating hWnd
    graphics_ptr = std::make_unique<Graphics>(hWnd);
}

Window::~Window() {
    DestroyWindow(hWnd);
}

std::optional<int> Window::processMessages() {
    MSG msg;

    while(PeekMessage(&msg, nullptr, 0,0 , PM_REMOVE)) {
        if(msg.message == WM_QUIT) {
            return msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return std::nullopt;
}

/* this procedure is to get window instance pointer by CREATESRTUCT
   and link it with winapi32 side and then set new window procedure 'HandleMsgThunk */
LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    /* we receive pointer to CREATESTRUCT by this message (in 'lParam') */
    if(msg == WM_NCCREATE) {
        const CREATESTRUCTW *const pCreate = reinterpret_cast<CREATESTRUCTW *>(lParam);
        Window *const pWnd = static_cast<Window *>(pCreate->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::HandleMsgThunk));

        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* this procedure is to invoke 'HandleMsg' procedure,
   we cann't use directly 'HandleMsg' as window procedure */
LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    Window *const pWnd = reinterpret_cast<Window *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    switch(msg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

        /* keyboard input */
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            /* 0x40000000 is a binary number with only one '1' on 30st position (we count index from right to left)
            This number indicates whether key was pressed previously or not. 'lParam' hold that information, but not only
            that, so we can check 'autorepeat' by using bit operation.
            (lParam & 0x40000000) -> 1 : key was previously pressed
            (lParam & 0x40000000) -> 0 : wasn't previously pressed  */

            /* here we don't want to use autorepeat, unless 'autorepeatIsEnabled' is true */
            if(!(lParam & 0x40000000) || keyboard.AutorepeatIsEnabled()) {
                keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
            }
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
            break;

        case WM_CHAR:
            keyboard.OnChar(static_cast<unsigned char>(wParam));
            break;

        /* if our window wasn't active (e.g. another window was creater) we want
        to clear our window state, because new messages will come to new window */
        case WM_KILLFOCUS:
            keyboard.ClearState();
            break;

        /* mouse input */
        case WM_MOUSEMOVE: {
            const POINTS pt = MAKEPOINTS(lParam);

            if(pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) {
                mouse.OnMouseMove(pt.x, pt.y);

                if(!mouse.IsInWindow()) {
                    SetCapture(hWnd);
                    mouse.OnMouseEnter();
                }                
            }
            else {
                if(wParam & (MK_LBUTTON | MK_RBUTTON)) {
                    mouse.OnMouseMove(pt.x, pt.y);
                }
                else {
                    ReleaseCapture();
                    mouse.OnMouseLeave();
                }
            }

            break;
        }

        case WM_LBUTTONDOWN: {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnRightPressed(pt.x, pt.y);
            break;
        }

        case WM_LBUTTONUP: {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnLeftPressed(pt.x, pt.y);
            break;
        }

        case WM_MOUSEWHEEL: {
            const POINTS pt = MAKEPOINTS(lParam);
            const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            mouse.OnWheelDelta(pt.x, pt.y, delta);
            /* if(GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
                mouse.OnWheelUp(pt.x, pt.y);
            }
            else if(GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
                mouse.OnWheelDown(pt.x, pt.y);
            } */
            break;
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Window exception

Window::Exception::Exception(int line, const char *file, HRESULT hr) noexcept
:   DXException(line, file),
    hr(hr) {}

const char *Window::Exception::what() const noexcept {
    std::ostringstream oss;

    oss << getType() << '\n'
    << "[Error code] " << getErrorCode() << '\n'
    << "[Description] " << getErrorString() << '\n'
    << getOriginString();

    /* we can't return oss.c_str() directly because
      it will not exist after what() function execution
      and we can't return pointer to nothing so the result
      of oss.str() will be stored in class memeber 'whatBuffer' */
    
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *Window::Exception::getType() const noexcept {
    return "DX Exception";
}

std::string Window::Exception::translateErrorCode(HRESULT hr) noexcept {
    char *pMsgBuf = nullptr;

    DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
    );

    if(nMsgLen == 0) {
        return "Unidentified error code";
    }

    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);

    return errorString;
}

HRESULT Window::Exception::getErrorCode() const noexcept {
    return hr;
}

std::string Window::Exception::getErrorString() const noexcept {
    return translateErrorCode(hr);
}

Graphics& Window::gfx() {
    return *graphics_ptr;
}

