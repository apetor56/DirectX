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

// #define DXWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
// #define DXWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())

#define DXWND_EXCEPT( hr ) Window::HrException( __LINE__,__FILE__,(hr) )
#define DXWND_LAST_EXCEPT() Window::HrException( __LINE__,__FILE__,GetLastError() )
#define DXWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )

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

    class Exception : public DXException
	{
		using DXException::DXException;
	public:
		static std::string TranslateErrorCode( HRESULT hr ) noexcept;
	};
	class HrException : public Exception
	{
	public:
		HrException( int line,const char* file,HRESULT hr ) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* getType() const noexcept override;
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
