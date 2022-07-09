#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "RWin.hpp"
#include "DXException.hpp"
#include <d3d11.h>
#include <vector>
#include <wrl.h>

class Graphics {
public:
    class Exception : public DXException
	{
		using DXException::DXException;
	};
	class HrException : public Exception
	{
	public:
		HrException( int line,const char* file,HRESULT hr,std::vector<std::string> infoMsgs = {} ) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException( int line,const char* file,std::vector<std::string> infoMsgs ) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* getType() const noexcept override;
	private:
		std::string reason;
	};

    Graphics(HWND hWnd);

    ~Graphics() = default;

    Graphics(const Graphics& graphics) = delete;

    Graphics& operator=(const Graphics& graphics) = delete;

    void endFrame();

    void clearBuffer(float red, float green, float blue) noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D11Device> device_ptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swap_ptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_ptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> view_ptr;
};

#endif // GRAPHICS_HPP